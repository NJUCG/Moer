#include "Mesh.h"
#include "FunctionLayer/Intersection.h"

Mesh::Mesh(std::shared_ptr<MeshData> _data, 
           std::shared_ptr<Material> _material,
           const Json & json):
    Entity(json),
    m_vertices(std::move(_data->m_vertices)),
    m_normals(std::move(_data->m_normals)),
    m_tangents(std::move(_data->m_tangents)),
    m_bitangents(std::move(_data->m_bitangents)),
    m_UVs(std::move(_data->m_UVs)),
    m_indices(std::move(_data->m_indices))
{   
    this->material = _material;
    
    auto maxX = m_vertices.row(0).maxCoeff(),
         minX = m_vertices.row(0).minCoeff(),
         maxY = m_vertices.row(1).maxCoeff(),
         minY = m_vertices.row(1).minCoeff(),
         maxZ = m_vertices.row(2).maxCoeff(),
         minZ = m_vertices.row(2).minCoeff();

    m_aabb = BoundingBox3f {
        Point3d {minX, minY, minZ},
        Point3d {maxX, maxY, maxZ}
    };

    BVH = std::make_shared<Bvh>(this);
}



Triangle Mesh::getTriangle(int idx) const {
    auto [i0, i1, i2] = m_indices[idx];
    auto p0 = eigenToPoint3d(m_vertices.col(i0)),
         p1 = eigenToPoint3d(m_vertices.col(i1)),
         p2 = eigenToPoint3d(m_vertices.col(i2));
    std::vector<Point3d> vertices;
    vertices.emplace_back(p0);
    vertices.emplace_back(p1);
    vertices.emplace_back(p2);
    return Triangle{vertices, material};     
}

std::optional<Intersection> Mesh::intersect(const Ray &r) const
{
    return BVH->Intersect(r);
}

void Mesh::apply()
{
    m_vertices = matrix->transformPoints(m_vertices);
    m_normals = matrix->transformNormals(m_normals);
//   Eigen::Matrix4d  matrix4D(matrix.);
//   matrix4D*this->m_vertices;
}

double Mesh::area() const
{
    // TODO
    return 0;
}

Intersection Mesh::sample(const Point2d &positionSample) const 
{
    // TODO
    return Intersection();
}

std::shared_ptr<Light> Mesh::getLight() const 
{
    return this->lightPtr;
}

void Mesh::setLight(std::shared_ptr<Light> light)
{
    this->lightPtr = light;
}

BoundingBox3f Mesh::WorldBound() const
{
    // TODO, store the bounding box
    return m_aabb;
}

RTCGeometry Mesh::toEmbreeGeometry(RTCDevice device) const 
{
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    float *vertices = (float *)rtcSetNewGeometryBuffer(
        geom, RTC_BUFFER_TYPE_VERTEX, 0, 
        RTC_FORMAT_FLOAT3,3 * sizeof(float), 
        m_vertices.cols()
    );

    //* brute-force copy
    for (int i = 0; i < m_vertices.cols(); ++i) {
        auto [x, y, z] = eigenToPoint3d(m_vertices.col(i));
        vertices[i * 3 + 0] = x;
        vertices[i * 3 + 1] = y;
        vertices[i * 3 + 2] = z;
    };

    unsigned *indices = (unsigned *)rtcSetNewGeometryBuffer(
        geom, RTC_BUFFER_TYPE_INDEX, 0, 
        RTC_FORMAT_UINT3, 3 * sizeof(unsigned),
        m_indices.size()
    );

    //* brute-force copy
    for (int i = 0; i < m_indices.size(); ++i) {
        auto [i0, i1, i2] = m_indices[i];
        indices[i * 3 + 0] = i0;
        indices[i * 3 + 1] = i1;
        indices[i * 3 + 2] = i2;
    }

    rtcCommitGeometry(geom);
    return geom;
}

EntitySurfaceInfo
Mesh::getEntitySurfaceInfo(int primID, Point2d _uv) const 
{
    //TODO add tangent and bitangent
    auto [u, v] = _uv;
    auto [i0, i1, i2] = m_indices[primID];
    auto p0 = eigenToPoint3d(m_vertices.col(i0)),
         p1 = eigenToPoint3d(m_vertices.col(i1)),
         p2 = eigenToPoint3d(m_vertices.col(i2));
    auto n0 = eigenToVector3d(m_normals.col(i0)),
         n1 = eigenToVector3d(m_normals.col(i1)),
         n2 = eigenToVector3d(m_normals.col(i2));
    auto uv0 = m_UVs[i0],
         uv1 = m_UVs[i1],
         uv2 = m_UVs[i2];

    Point3d position = (1 - u - v) * p0
        + u * p1
        + v * p2;
    Normal3d normal = (1 - u - v) * n0
        + u * n1
        + v * n2;
    Point2d uv = (1 - u - v) * uv0
        + u * uv1
        + v * uv2;
    return {position, normal, Normal3d(), Normal3d(), uv};
}

std::optional<Intersection> Mesh::getIntersectionFromRayHit(const UserRayHit1 &rayhit) const
{
    auto primID = rayhit.hit.primID;
    double u = rayhit.hit.u,
           v = rayhit.hit.v;

    auto [i0, i1, i2] = m_indices[primID];
    auto p0 = eigenToPoint3d(m_vertices.col(i0)),
         p1 = eigenToPoint3d(m_vertices.col(i1)),
         p2 = eigenToPoint3d(m_vertices.col(i2));
    auto n0 = eigenToVector3d(m_normals.col(i0)),
         n1 = eigenToVector3d(m_normals.col(i1)),
         n2 = eigenToVector3d(m_normals.col(i2));
    auto uv0 = m_UVs[i0],
         uv1 = m_UVs[i1],
         uv2 = m_UVs[i2];

    Point3d position = (1 - u - v) * p0
        + u * p1
        + v * p2;
    Normal3d normal = (1 - u - v) * n0
        + u * n1
        + v * n2;
    Point2d uv = (1 - u - v) * uv0
        + u * uv1
        + v * uv2;



    //todo tangent and bitangent
    Intersection its;
    its.t = rayhit.ray.tfar;
    its.object = this;
    its.material = this->material;
    its.position = position;
    its.geometryNormal = normal;
    its.shFrame = Frame{normal};
    its.uv = uv;
    return std::make_optional(its);
}

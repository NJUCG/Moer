#include "Mesh.h"

#include <utility>
#include "FunctionLayer/Intersection.h"
using MeshMap = std::unordered_map<const MeshData *,std::shared_ptr<TraceableMesh>>;
RTCRay toRTCRay(const Ray &ray);

class TraceableMesh {
public:
    std::shared_ptr<const MeshData> data;
    RTCGeometry geometry;
    RTCScene instanceScene = nullptr ;
    RTCScene scene = nullptr;
    const Eigen::MatrixXd normals;
public:
    TraceableMesh( const std::shared_ptr<const  MeshData>& data) :normals(data->m_normals){
        this->data =  std::make_shared<MeshData>(*data);

    }
    void initEmbree(RTCDevice device){
        if(scene) return;
        RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
        float *vertices = (float *)rtcSetNewGeometryBuffer(
            geom, RTC_BUFFER_TYPE_VERTEX, 0,
            RTC_FORMAT_FLOAT3, 3 * sizeof(float),
            data->m_vertices.cols());
        //* brute-force copy
        for (int i = 0; i < data->m_vertices.cols(); ++i) {
            Eigen::DenseBase<Eigen::MatrixXd>::ConstColXpr col =data->m_vertices.col(i);
            auto [x, y, z] = eigenToPoint3d(data->m_vertices.col(i));
            vertices[i * 3 + 0] = x;
            vertices[i * 3 + 1] = y;
            vertices[i * 3 + 2] = z;
        };

        unsigned *indices = (unsigned *)rtcSetNewGeometryBuffer(
            geom, RTC_BUFFER_TYPE_INDEX, 0,
            RTC_FORMAT_UINT3, 3 * sizeof(unsigned),
            data->m_indices.size());
        //* brute-force copy
        for (int i = 0; i < data->m_indices.size(); ++i) {
            auto [i0, i1, i2] = data->m_indices[i];
            indices[i * 3 + 0] = i0;
            indices[i * 3 + 1] = i1;
            indices[i * 3 + 2] = i2;
        }
        rtcCommitGeometry(geom);
        scene = rtcNewScene(device);
        rtcAttachGeometry(scene,geom);
        rtcReleaseGeometry(geom);
        rtcCommitScene(scene);
    }
    static std::shared_ptr<TraceableMesh> getTraceableMesh(std::shared_ptr<const MeshData> meshData){
        if(map.count(meshData.get()))
            return map.at(meshData.get());
        auto traceableMesh = std::make_shared<TraceableMesh>(meshData);
        map[meshData.get()] = traceableMesh;
        return traceableMesh;
    }
    std::optional<Intersection> intersect(const Ray & ray,std::shared_ptr< TransformMatrix3D> transform ){
        RTCRayHit rayhit;
        rayhit.ray= toRTCRay(ray);
        rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
        RTCIntersectContext ictx;
        rtcInitIntersectContext(&ictx);
        rtcIntersect1(scene,&ictx,&rayhit);
        if(rayhit.hit.geomID== RTC_INVALID_GEOMETRY_ID)
            return std::nullopt;
        auto primID = rayhit.hit.primID;
        double u = rayhit.hit.u,
               v = rayhit.hit.v;

        auto [i0, i1, i2] = data->m_indices[primID];
        //std::cout<<primID<<" ";
        auto p0 = eigenToPoint3d(data->m_vertices.col(i0)),
             p1 = eigenToPoint3d(data->m_vertices.col(i1)),
             p2 = eigenToPoint3d(data->m_vertices.col(i2));
        auto n0 = eigenToVector3d(data->m_normals.col(i0)),
             n1 = eigenToVector3d(data->m_normals.col(i1)),
             n2 = eigenToVector3d(data->m_normals.col(i2));
        auto uv0 = data->m_UVs[i0],
             uv1 = data->m_UVs[i1],
             uv2 = data->m_UVs[i2];

        Point3d position = (1 - u - v) * p0 + u * p1 + v * p2;
        Normal3d normal = (1 - u - v) * n0 + u * n1 + v * n2;
        Point2d uv = (1 - u - v) * uv0 + u * uv1 + v * uv2;

        position = transform->operator*(position);
        normal = transform->operator*(normal);
        auto val = reinterpret_cast<uintptr_t>(data.get());
        unsigned char r = (val >> 16) & 0xFF;
        unsigned char g = (val >> 8) & 0xFF;
        unsigned char b = val & 0xFF;

        //todo tangent and bitangent
        Intersection its;
        its.t = rayhit.ray.tfar;
        its.position = position;
        its.geometryNormal = normal;
        its.shFrame = Frame{normal};
        its.uv = uv;
        return std::make_optional(its);
    }
private:
    static MeshMap map;
};

MeshMap  TraceableMesh::map = MeshMap();

Mesh::Mesh(std::shared_ptr<MeshData> _data,
           std::shared_ptr<Material> _material,
           const Json &json) : Entity(json),
                               m_indices(_data->m_indices),
                               m_vertices(_data->m_vertices),
                               meshData(TraceableMesh::getTraceableMesh(_data))
                              {
    inverseMatrix = std::make_shared<TransformMatrix3D>(matrix->getInverse());
    this->material = _material;
    auto maxX = meshData->data->m_vertices.row(0).maxCoeff(),
         minX = meshData->data->m_vertices.row(0).minCoeff(),
         maxY = meshData->data->m_vertices.row(1).maxCoeff(),
         minY = meshData->data->m_vertices.row(1).minCoeff(),
         maxZ = meshData->data->m_vertices.row(2).maxCoeff(),
         minZ = meshData->data->m_vertices.row(2).minCoeff();
    Point3d vertices[8];
    for(int i = 0 ;i< 8 ; i++){
        vertices[i].x = (i & 4)?minX:maxX;
        vertices[i].y = (i & 2)?minY:maxY;
        vertices[i].z = (i & 1)?minZ:maxZ;
        vertices[i] = matrix ->operator*( vertices[i]);
        minX = std::min(minX,vertices[i].x);
        minY = std::min(minY,vertices[i].y);
        minZ = std::min(minZ,vertices[i].z);
        maxX = std::max(maxX,vertices[i].x);
        maxY = std::max(maxY,vertices[i].y);
        maxZ = std::max(maxZ,vertices[i].z);
    }
    m_aabb =  BoundingBox3f{
                Point3d{minX, minY, minZ},
               Point3d{maxX, maxY, maxZ}};
}

Triangle Mesh::getTriangle(int idx) const {
    auto [i0, i1, i2] = meshData->data->m_indices[idx];
    auto p0 = eigenToPoint3d(meshData->data->m_vertices.col(i0)),
         p1 = eigenToPoint3d(meshData->data->m_vertices.col(i1)),
         p2 = eigenToPoint3d(meshData->data->m_vertices.col(i2));
    std::vector<Point3d> vertices;
    vertices.emplace_back(p0);
    vertices.emplace_back(p1);
    vertices.emplace_back(p2);
    return Triangle{vertices, material};
}

Ray getInverseRay(const Ray & ray,std::shared_ptr<TransformMatrix3D> inverseMatrix){
    Point3d  convertOrigin = inverseMatrix->operator*(ray.origin);
    Vec3d convertDir=  normalize(inverseMatrix->operator*(ray.direction));// normalize((inverse(M) * make_vec4(D, 0)).xyz);
    return {convertOrigin,convertDir,ray.timeMin,ray.timeMax};
}

std::optional<Intersection> Mesh::intersect(const Ray &r) const {
    Ray ray = getInverseRay(r,inverseMatrix);
    auto its =  meshData->intersect(ray,matrix);
    if(its) {
        its->object = this;
        its->material = material;
    }
    return its;
}

void Mesh::apply() {
    //
}

double Mesh::area() const {
    // TODO
    return 0;
}

Intersection Mesh::sample(const Point2d &positionSample) const {
    // TODO
    return Intersection();
}

BoundingBox3f Mesh::WorldBound() const {
    return m_aabb;
}

RTCGeometry Mesh::toEmbreeGeometry(RTCDevice device) const {
    meshData->initEmbree(device);
    return Entity::toEmbreeGeometry(device);
}

std::optional<Intersection> Mesh::getIntersectionFromRayHit(const UserRayHit1 &rayhit) const {
    return Entity::getIntersectionFromRayHit(rayhit);

}
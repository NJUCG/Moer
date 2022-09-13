#include "Mesh.h"
#include "FunctionLayer/Intersection.h"

Mesh::Mesh(std::shared_ptr<MeshData> _data, 
           std::shared_ptr<Material> _material): 
    m_vertices(_data->m_vertices),
    m_normals(_data->m_normals),
    m_tangents(_data->m_tangents),
    m_bitangents(_data->m_bitangents),
    m_UVs(_data->m_UVs),
    m_indices(_data->m_indices)
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
}

double Mesh::area() const
{
    // TODO
}

Intersection Mesh::sample(const Point2d &positionSample) const 
{
    // TODO
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
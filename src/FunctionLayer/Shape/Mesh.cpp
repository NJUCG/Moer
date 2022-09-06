#include "Mesh.h"
#include "FunctionLayer/Intersection.h"

Mesh::Mesh(std::shared_ptr<MeshData> _data, 
           std::shared_ptr<Material> _material)
    : data(_data)
{
    this->material = _material;

    //* Build bvh for the mesh
    m_triangles.reserve(data->getTriangleNum());
    for (int i = 0; i < data->getTriangleNum(); ++i) {
        std::vector<Point3d> vertices;
        auto indices = data->getTriangleIndiceAt(i);
        vertices.emplace_back(data->getVertexAt(indices[0]));
        vertices.emplace_back(data->getVertexAt(indices[1]));
        vertices.emplace_back(data->getVertexAt(indices[2]));
        
        m_triangles.emplace_back(
            std::make_shared<Triangle>(
                vertices,
                this->material  
            )
        );  
    }

    BVH = std::make_shared<Bvh>(m_triangles);
}

std::optional<Intersection> Mesh::intersect(const Ray &r) const
{
    if(BVH)
        return BVH->Intersect(r);
    
    std::optional<Intersection> result = std::nullopt;
    for (int i = 0; i < data->getTriangleNum(); ++i) {
        std::vector<Point3d> vertices;
        auto indices = data->getTriangleIndiceAt(i);
        vertices.emplace_back(data->getVertexAt(indices[0]));
        vertices.emplace_back(data->getVertexAt(indices[1]));
        vertices.emplace_back(data->getVertexAt(indices[2]));
        Triangle triangle {vertices, this->material};

        auto intersect = triangle.intersect(r);
        if (intersect.has_value()) {
            if (!result.has_value())
                result = intersect;
            else {
                if (intersect->t < result->t)
                    result = intersect;
            }
        }
    }
    if (result)
        result.value().object = this;
    return result;
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
    return data->getAABB();
}
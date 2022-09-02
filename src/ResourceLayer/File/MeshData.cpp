#include "MeshData.h"

Point3d MeshData::getVertexAt(int i) const {
    auto coords = m_vertices.col(i);
    return Point3d(coords[0], coords[1], coords[2]);
}

Normal3d MeshData::getNormalAt(int i) const {
    auto coords = m_normals.col(i);
    return Normal3d(coords[0], coords[1], coords[2]);
}

// TODO
Point2d MeshData::getUvAt(int i) const {

}

// TODO
Vec3d MeshData::getTangentAt(int i) const {

}

// TODO
Vec3d MeshData::getBitangentAt(int i) const {

}

Point3i MeshData::getTriangleIndiceAt(int i) const {
    return m_indices[i];
}

int MeshData::getTriangleNum() const {
    return m_indices.size();
}

BoundingBox3f MeshData::getAABB() const {
    return m_aabb;
}
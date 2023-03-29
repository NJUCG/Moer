#include "Cube.h"
#include "FastMath.h"

Cube::Cube(const Json &json) : Entity(json) {
    position = matrix->getTranslate() * Point3d(.0f);
    scale = Matrix4x4::scale(0.5) * matrix->getScale();
    rotation = matrix->getRotate();
}

std::optional<Intersection> Cube::intersect(const Ray &r) const {
    Matrix4x4 invRotation = rotation.inverse();

    //* ori_ is the r.origin in cube coordinate system
    //* dir_ is the r.direction in cube coordinate system 
    //* That is, rotate the ray other than the cube
    auto ori_ = invRotation * (r.origin - position);
    auto dir_ = invRotation * r.direction;

    //* Now, the cube is an axis-aligned box
    auto pMin = - (scale * Point3d(1)),
         pMax = scale * Point3d(1);

    auto tmin = r.timeMin,
         tmax = r.timeMax;


    //todo divide 0?
    for (int i = 0; i < 3; ++i) {
        auto invD = 1 / dir_[i];
        if (invD >= 0) {
            tmin = std::max(tmin, (pMin[i] - ori_[i]) * invD);
            tmax = std::min(tmax, (pMax[i] - ori_[i]) * invD);
        } else {
            tmax = std::min(tmax, (pMin[i] - ori_[i]) * invD);
            tmin = std::max(tmin, (pMax[i] - ori_[i]) * invD);
        }
    }

    if (tmin > tmax)
        return std::nullopt;

    auto computeNormalInLocal = [&pMin, &pMax](Vec3d hitpoint_)
    {
        int minDimension = -1;
        double minBias = 1e10;
        for (int i = 0; i < 3; ++i) {
            if (fm::abs(pMin[i] - hitpoint_[i]) < minBias) {
                minDimension = 2 * i;
                minBias = fm::abs(fm::abs(pMin[i] - hitpoint_[i]));
            }
            if (fm::abs(pMax[i] - hitpoint_[i]) < minBias) {
                minDimension = 2 * i + 1;
                minBias = fm::abs(pMax[i] - hitpoint_[i]);
            }
        }
        assert(minDimension != -1);
        Vec3d normal{0};
        normal[minDimension/2] = (minDimension%2) ? 1 : -1;
        return normal;
    };

    Intersection its;
    if (tmin > r.timeMin && tmin < r.timeMax) {
        its.t = tmin;
        Vec3d hitpoint_ = ori_ + its.t * dir_;
        its.geometryNormal = rotation * computeNormalInLocal(hitpoint_);
        its.position = r.at(its.t);
        its.object = this;
        its.shFrame = Frame{its.geometryNormal};
        its.material = material;
        return std::make_optional(its);
    } else if (tmax > r.timeMin && tmax < r.timeMax) {
        its.t = tmax;
        Vec3d hitpoint_ = ori_ + its.t * dir_;
        its.geometryNormal = rotation * computeNormalInLocal(hitpoint_);
        its.position = r.at(its.t);
        its.object = this;
        its.shFrame = Frame{its.geometryNormal};
        its.material = material;
        return std::make_optional(its);
    }
    return std::nullopt;
}

double Cube::area() const {
    Vec3d edge = scale * Vec3d(1);
    return 2 * (edge.x * edge.y + edge.x * edge.z + edge.y * edge.z);
}

// todo
Intersection Cube::sample(const Point2d &positionSample) const {
    return Intersection();
}

BoundingBox3f Cube::WorldBound() const {
    BoundingBox3f box;
    Vec3d edge = scale * Vec3d(1);

    for (int i = 0; i < 8; ++i) {
        Point3d vertex = position +  rotation * Vec3d(
                (i & 1 ? -edge.x : edge.x),
                (i & 2 ? -edge.y : edge.y),
                (i & 4 ? -edge.z : edge.z)
            );

        box = BoundingBoxPointUnion(box, vertex);
    }
    return box;
}

void Cube::apply() {
    //* do nothing
    return;
}
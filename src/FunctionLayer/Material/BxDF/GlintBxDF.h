#pragma once
#include "BxDF.h"
#include "MicrofacetDistribution.h"
#include "../../Intersection.h"
#include "CoreLayer/Geometry/BoundingBox.h"
#include <random>
#include <VecMat.h>

#define MAX_QUERY_DEPTH 1000
#define SPATIAL_SAMPLE_NUM 10000000
#define DIRECTION_SAMPLE_NUM 10000000
#define EPSILON 0.1f
#define GAMMA 0.087f
#define Spherical2Cartesian(dir) Point3d(cosf(dir.x) * sinf(dir.y), sinf(dir.x) * sinf(dir.y), cosf(dir.y))

// init posibilities in each depth query
static std::vector<Point3d> queryPs;
static int initQueryPs = []() {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_real_distribution<double> distr(0.f, 1.f);
    double x, y, z, variance, max;

    for (int i = 0; i < MAX_QUERY_DEPTH;) {
        x = distr(eng);
        y = (1 - x) * distr(eng);
        z = (1 - x - y) * distr(eng);
        variance = ((x - 0.25) * (x - 0.25) + (y - 0.25) * (y - 0.25) + (z - 0.25) * (z - 0.25) + (0.75 - x - y - z) * (0.75 - x - y - z)) / 4;
        // when i small, the variance should also be small
        max = 0.05 + (i * 10 / MAX_QUERY_DEPTH * 0.1);
        if (variance < max * max) {
            queryPs.push_back(Point3d(x, y, z));
            i++;
        }
    }

    return 0;
}();

// dirction trianle for dirction query
struct DirTriangle {
    Point2d verts[3];

    DirTriangle() {
        verts[0] = Point2d(0.f, 0.f), verts[1] = Point2d(0.f, 0.f), verts[2] = Point2d(0.f, 0.f);
    }
    DirTriangle(const Point2d &p1, const Point2d &p2, const Point2d &p3) {
        verts[0] = p1, verts[1] = p2, verts[2] = p3;
    }
    // top point's x value is meaningless
    bool isTopPoint(int index) const {
        return fabs(verts[index].y - M_PI / 2) < 1e-4;
    }
    Point2d edgeMidPoint(int index) const {
        int i = index, j = (index + 1) % 3;
        if (isTopPoint(i))
            return Point2d(verts[j].x, 0.5 * (verts[i].y + verts[j].y));
        else if (isTopPoint(j))
            return Point2d(verts[i].x, 0.5 * (verts[i].y + verts[j].y));
        else
            return Point2d(0.5 * (verts[i].x + verts[j].x), 0.5 * (verts[i].y + verts[j].y));
    }

    Point2d operator[](int index) const {
        assert(index >= 0 && index < 3);
        return verts[index];
    }
};

// dirction query struct
struct ConicQuery {
    Vec3d wi, wo;
    vecmat::mat33d C;

    ConicQuery(const Vec3d &_wi, const Vec3d &_wo) : wi(_wi), wo(_wo) {
        Vec3d x = normalize(cross(wi, wo)), y = normalize(wi - wo), z = normalize(wi + wo);
        double lambda1 = (dot(wi, wo) + cosf(GAMMA)) / (1 - cosf(GAMMA)), lambda2 = 1.f / (tanf(GAMMA / 2) * tanf(GAMMA / 2));
        vecmat::mat33d Q({x[0], y[0], z[0], x[1], y[1], z[1], x[2], y[2], z[2]}),
            A({lambda1, 0.f, 0.f, 0.f, lambda2, 0.f, 0.f, 0.f, -1.f}),
            QTranspose;
        C = Q;
        C = C * A;
        QTranspose = Q.transpose();
        C = C * QTranspose;
    }

    //judge if a point falls in cone
    bool inConic(const Point2d &p) const {
        Point3d dir = Spherical2Cartesian(p);
        vecmat::mat<3, 1, double> m({dir[0], dir[1], dir[2]});
        return (m.transpose() * C * m).rows[0][0] < 0.f;
    }

    //judge if triangle intersects cone
    //not used yet
    bool IntersectConic(const DirTriangle &_tri) const {
        Point3d tri[3] = {Spherical2Cartesian(_tri[0]), Spherical2Cartesian(_tri[1]), Spherical2Cartesian(_tri[2])};
        vecmat::mat<3, 1, double> c, d;
        Vec3d _c, _d;
        double param_a, param_b, param_c, axis;
        for (int i = 0; i < 3; i++) {
            _c = tri[i] + tri[(i + 1) % 3] - Point3d(0.f, 0.f, 0.f);
            _d = tri[i] - tri[(i + 1) % 3];
            c = vecmat::mat<3, 1, double>(_c[0], _c[1], _c[2]);
            d = vecmat::mat<3, 1, double>(_d[0], _d[1], _d[2]);
            // The three coefficients of the equation
            param_a = (d.transpose() * C * d).rows[0][0];
            param_b = (d.transpose() * C * c).rows[0][0];
            param_b *= 2.f;
            param_c = (c.transpose() * C * c).rows[0][0];

            // root in [-1, 1]
            if ((param_a - param_b + param_c) * (param_a + param_b + param_c) < 0.f)
                return true;
            else {
                axis = -param_b / (2 * param_a);
                if (axis < -1.f || axis > 2.f || 4.f * param_a * param_c - param_b * param_b >= 0.f)
                    continue;
                else
                    return true;
            }
        }
        return false;
    }

    bool conicContain(const DirTriangle &_tri) const {
        return (inConic(_tri[0]) && inConic(_tri[1]) && inConic(_tri[2]));
    }
};

// bfs tree node: range and particles number in it
typedef std::pair<BoundingBox2d, uint32_t> SpatialNode;
typedef std::pair<DirTriangle, uint32_t> DirectionNode;

class GlintBxDF : public BxDF {
public:
    GlintBxDF(Vec3d _eta, Vec3d _k, Spectrum _albedo, double _uRoughness, double _vRoughness, Intersection _it,
              std::shared_ptr<MicrofacetDistribution> _distrib);

    Spectrum f(const Vec3d &out, const Vec3d &in) const override;

    double pdf(const Vec3d &out, const Vec3d &in) const override;

    BxDFSampleResult sample(const Vec3d &out, const Point2d &sample) const override;

    double count_spatial(BoundingBox2d &queryBox) const;

    double count_direction(const Vec3d &wi, const Vec3d &wo) const;

    [[nodiscard]] double getRoughness() const override { return (alphaXY[0] + alphaXY[1]) / 2.; }

protected:
    std::shared_ptr<MicrofacetDistribution> distrib;
    Vec2d alphaXY;
    Vec3d eta;
    Vec3d k;
    Spectrum albedo;
    Intersection it;
};

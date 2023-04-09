#include "MicrofacetDistribution.h"
#include "CoreLayer/Geometry/Frame.h"

#include "BxDF.h"

std::shared_ptr<MicrofacetDistribution> LoadDistributionFromJson(const Json & json){
    if(!json.contains("distribution"))
        return std::make_shared<BeckmannDistribution>();
    std::string distribStr = json["distribution"];
    if(distribStr == "beckmann")
        return std::make_shared<BeckmannDistribution>();
    else if(distribStr == "ggx")
        return std::make_shared <GGXDistribution>(true);
    {}
}


MicrofacetDistribution::~MicrofacetDistribution( ) noexcept {}

double MicrofacetDistribution::Pdf(const Vec3d & wo, const Vec3d & wh, const Vec2d &alphaXY) const {
    if(wh.z<0) return 0 ;
    if (sampleVisibleArea)
        return D(wh, alphaXY) * G1(wo,alphaXY) * absDot(wo, wh) / AbsCosTheta(wo);
    else
        return D(wh, alphaXY) * AbsCosTheta(wh);
}

double BeckmannDistribution::roughnessToAlpha(double roughness) const {
    roughness = std::max(roughness, (double)1e-3);
    return roughness;
}

double BeckmannDistribution::D(const Vec3d & wh, const Vec2d & alphaXY) const {
    double alphaX =alphaXY.x;
    double alphay =alphaXY.y;
    double tan2Theta = Tan2Theta(wh);
    if (std::isinf(tan2Theta)) return 0.;
    double cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);
    return std::exp(-tan2Theta * (Cos2Phi(wh) / (alphaX * alphaX) +
                                  Sin2Phi(wh) / (alphay * alphay))) /
           (M_PI * alphaX * alphay * cos4Theta);
}

double BeckmannDistribution::Lambda(const Vec3d & w, const Vec2d & alphaXY) const {
    double alphaX =alphaXY.x;
    double alphay =alphaXY.y;

    double absTanTheta = std::abs(TanTheta(w));
    if (std::isinf(absTanTheta)) return 0.;
    // Compute _alpha_ for direction _w_
    double alpha =
            std::sqrt(Cos2Phi(w) * alphaX * alphaX + Sin2Phi(w) * alphay * alphay);
    double a = 1 / (alpha * absTanTheta);
    if (a >= 1.6f) return 0;
    return (1 - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
}

Vec3d BeckmannDistribution::Sample_wh(const Vec3d & wo, Point2d u, const Vec2d & alphaXY) const {
    if (CosTheta(wo) < 0) {
        return Sample_wh(-wo, u,alphaXY);
    }
    double alphaX =alphaXY.x;
    double alphay =alphaXY.y;

    //random sample  no importance sampling
    double tan2Theta;
    double phi;
    //todo add support sample visible area
    if(!sampleVisibleArea){
        if(alphaX == alphay){
            tan2Theta = -std::log(1-u[0]) * alphaX * alphaX ;
            phi = u[1] * 2 * M_PI;
        }
        else {

            phi = std::atan(alphay / alphaX *
                            std::tan(2 * M_PI * u[1] + 0.5 * M_PI));
            if (u[1] > 0.5) phi += M_PI;
            double sinPhi = std::sin(phi), cosPhi = std::cos(phi);
            double alphaX2 = alphaX * alphaX, alphay2 = alphay * alphay;
            tan2Theta = -std::log(1-u[0]) /
                        (cosPhi * cosPhi / alphaX2 + sinPhi * sinPhi / alphay2);
        }
        double cosTheta = std::sqrt(1 / (1+tan2Theta));
        double sinTheta = std::sqrt(1 / (1+1/tan2Theta));
        Vec3d  wh = Vec3d(sinTheta*std::cos(phi),sinTheta*std::sin(phi),cosTheta);
        return wh;
    }
        // see https://hal.inria.fr/hal-00996995v1/document // todo
    else {

    }
}

std::string BeckmannDistribution::ToString( ) const {
    return std::string();
}

double GGXDistribution::G(const Vec3d & wo, const Vec3d & wi, const Vec2d & alphaXY) const {
    return G1(wo,alphaXY) * G1(wi,alphaXY);
}

double GGXDistribution::roughnessToAlpha(double roughness) const {
    return roughness;
}

double GGXDistribution::D(const Vec3d & wh, const Vec2d & alphaXY) const {
    double alphaX = alphaXY.x;
    double alphaY = alphaXY.y;
    double ax2 = alphaX * alphaX;
    double ay2 = alphaY * alphaY;
    Vec3d  wh2 = wh * wh;
    double D = M_PI * alphaX *alphaY * pow(wh2.x/ax2+wh2.y/ay2+wh2.z,2);
    return 1/D;
}

double GGXDistribution::Lambda(const Vec3d & w, const Vec2d & alphaXY) const {
    double ax2 =  alphaXY.x * alphaXY.x;
    double ay2 = alphaXY.y * alphaXY.y;
    Vec3d v2 = w * w;
    double Lambda = (-1 + sqrt(1 + (v2.x * ax2 + v2.y * ay2) / v2.z)) / 2;
    return Lambda;
}

Vec3d GGXDistribution::Sample_wh(const Vec3d & wo, Point2d u, const Vec2d & alphaXY) const {
    u.x= std::max(0.01,std::min(0.99,u.x));
    u.y= std::max(0.01,std::min(0.99,u.y));
    double alphaX = alphaXY.x,alphaY =alphaXY.y;
    if (CosTheta(wo) < 0) {
        return Sample_wh(-wo, u,alphaXY);
    }
    if(sampleVisibleArea){
        //see https://jcgt.org/published/0007/04/01/slides.pdf
        // Transform the incoming direction to the "hemisphere configuration".
        Vec3d hemisphereDirOut= normalize(Vec3d(alphaX * wo.x, alphaY * wo.y, wo.z));
        // Parameterization of the projected area of a hemisphere.
        double r = sqrt(u.x);
        double phi = 2 * M_PI * u.y;
        double t1 = r * cos(phi);
        double t2 = r * sin(phi);
        // Vertically scale the position of a sample to account for the projection.
        double s = (1 + hemisphereDirOut.z) / 2;
        t2 = (1 - s) * sqrt(1 - t1 * t1) + s * t2;
        // Point in the disk space
        Vec3d diskN{t1, t2, sqrt(std::max(0.0, 1 - t1*t1 - t2*t2))};
        // Reprojection onto hemisphere -- we get our sampled normal in hemisphere space.
        Vec3d T1 = normalize(Vec3d(-hemisphereDirOut.y,hemisphereDirOut.x,0));
        Vec3d T2 = cross(hemisphereDirOut,T1);
        Vec3d hemisphereN =  t1 * T1 + t2 * T2 + diskN.z * hemisphereDirOut;

        // Transforming the normal back to the ellipsoid configuration
        return normalize(Vec3d(alphaX * hemisphereN.x, alphaY *  hemisphereN.y, std::max(0.0, hemisphereN.z)));
    }
    else {
        double cosTheta, phi = (2 * M_PI) * u[1];
        if (alphaX == alphaY) {
            double tanTheta2 = alphaX * alphaY * u[0] / (1.0f - u[0]);
            cosTheta = 1 / std::sqrt(1 + tanTheta2);
        } else {
            phi =
                    std::atan(alphaY / alphaX * std::tan(2 * M_PI * u[1] + .5f * M_PI));
            if (u[1] > .5f) phi += M_PI;
            double sinPhi = std::sin(phi), cosPhi = std::cos(phi);
            const double alphaX2 = alphaX * alphaX, alphaY2 = alphaY * alphaY;
            const double alpha2 =
                    1 / (cosPhi * cosPhi / alphaX2 + sinPhi * sinPhi / alphaY2);
            double tanTheta2 = alpha2 * u[0] / (1 - u[0]);
            cosTheta = 1 / std::sqrt(1 + tanTheta2);
        }
        double sinTheta =
                std::sqrt(std::max((double )0., (double )1. - cosTheta * cosTheta));
        Vec3d wh = Vec3d(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
        return  wh;
    }
}


std::string GGXDistribution::ToString( ) const {
    return std::string();
}




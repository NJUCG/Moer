#include "MicrofacetDistribution.h"
#include "BxDF.h"



std::shared_ptr<MicrofacetDistribution> LoadDistributionFromJson(const Json & json){
    if(!json.contains("distribution"))
        return std::make_shared<BeckmannDistribution>();
    std::string distribStr = json["distribution"];
    if(distribStr == "beckmann")
        return std::make_shared<BeckmannDistribution>();
    else //todo
    {}
}


MicrofacetDistribution::~MicrofacetDistribution( ) noexcept {}

double MicrofacetDistribution::Pdf(const Vec3d & wo, const Vec3d & wh, const Vec2d &alphaXY) const {
    if (sampleVisibleArea)
        return D(wh, alphaXY) * G1(wo,alphaXY) * absDot(wo, wh) / AbsCosTheta(wo);
    else
        return D(wh, alphaXY) * AbsCosTheta(wh);
}

double BeckmannDistribution::roughnessToAlpha(double roughness) const {
    roughness = std::max(roughness, (double)1e-3);
    return roughness;
    double x = std::log(roughness);
    return 1.62142f + 0.819955f * x + 0.1734f * x * x +
           0.0171201f * x * x * x + 0.000640711f * x * x * x * x;
}

double BeckmannDistribution::D(const Vec3d & wh, const Vec2d & alphaXY) const {
    double alphax =alphaXY.x;
    double alphay =alphaXY.y;
    double tan2Theta = Tan2Theta(wh);
    if (std::isinf(tan2Theta)) return 0.;
    double cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);
    return std::exp(-tan2Theta * (Cos2Phi(wh) / (alphax * alphax) +
                                  Sin2Phi(wh) / (alphay * alphay))) /
           (M_PI * alphax * alphay * cos4Theta);
}

double BeckmannDistribution::Lambda(const Vec3d & w, const Vec2d & alphaXY) const {
    double alphax =alphaXY.x;
    double alphay =alphaXY.y;

    double absTanTheta = std::abs(TanTheta(w));
    if (std::isinf(absTanTheta)) return 0.;
    // Compute _alpha_ for direction _w_
    double alpha =
            std::sqrt(Cos2Phi(w) * alphax * alphax + Sin2Phi(w) * alphay * alphay);
    double a = 1 / (alpha * absTanTheta);
    if (a >= 1.6f) return 0;
    return (1 - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
}

Vec3d BeckmannDistribution::Sample_wh(const Vec3d & wo, const Point2d & u, const Vec2d & alphaXY) const {
    double alphax =alphaXY.x;
    double alphay =alphaXY.y;

    //random sample  no importance sampling
    double tan2Theta;
    double phi;
    //todo add support sample visible area
    if(!sampleVisibleArea || true){
        if(alphax == alphay){
            tan2Theta = -alphax * alphax * std::log(1-u[0]);
            phi = u[1] * 2 * M_PI;
        }
        else {

        }
        double cosTheta = std::sqrt(1 / (1+tan2Theta));
        double sinTheta = std::sqrt(1 / (1+1/tan2Theta));
        Vec3d  wh = Vec3d(sinTheta*std::cos(phi),sinTheta*std::sin(phi),cosTheta);
        if(dot(wo,wh)<0) wh = -wh;
        return wh;
    }
        // see https://hal.inria.fr/hal-00996995v1/document // todo
    else {

    }
}

std::string BeckmannDistribution::ToString( ) const {
    return std::string();
}

double TrowbridgeReitzDistribution::roughnessToAlpha(double roughness) const {
    return 0;
}

double TrowbridgeReitzDistribution::D(const Vec3d & wh, const Vec2d & alphaXY) const {
    return 0;
}

double TrowbridgeReitzDistribution::Lambda(const Vec3d & w, const Vec2d & alphaXY) const {
    return 0;
}

Vec3d TrowbridgeReitzDistribution::Sample_wh(const Vec3d & wo, const Point2d & u, const Vec2d & alphaXY) const {
    return Vec3d();
}

std::string TrowbridgeReitzDistribution::ToString( ) const {
    return std::string();
}




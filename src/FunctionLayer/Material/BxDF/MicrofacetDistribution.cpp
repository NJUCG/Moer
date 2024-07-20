#include "MicrofacetDistribution.h"
#include "CoreLayer/Geometry/Frame.h"

#include "BxDF.h"
#include "FastMath.h"

std::shared_ptr<MicrofacetDistribution> LoadDistributionFromJson(const Json &json) {
    if (!json.contains("distribution"))
        return std::make_shared<BeckmannDistribution>();
    std::string distribStr = json["distribution"];
    if (distribStr == "beckmann")
        return std::make_shared<BeckmannDistribution>();
    else if (distribStr == "ggx")
        return std::make_shared<GGXDistribution>(true);
    {}
}

MicrofacetDistribution::~MicrofacetDistribution() noexcept {}

double MicrofacetDistribution::Pdf(const Vec3d &wo, const Vec3d &wh, const Vec2d &alphaXY) const {
    if (sampleVisibleArea)
        return D(wh, alphaXY) * G1(wo, alphaXY) * absDot(wo, wh) / AbsCosTheta(wo);
    else
        return D(wh, alphaXY) * AbsCosTheta(wh);
}

double BeckmannDistribution::roughnessToAlpha(double roughness) const {
    roughness = std::max(roughness, (double)1e-3);
    return roughness;
}

double BeckmannDistribution::D(const Vec3d &wh, const Vec2d &alphaXY) const {
    double alphaX = alphaXY.x;
    double alphay = alphaXY.y;
    double tan2Theta = Tan2Theta(wh);
    if (std::isinf(tan2Theta)) return 0.;
    double cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);
    return std::exp(-tan2Theta * (Cos2Phi(wh) / (alphaX * alphaX) +
                                  Sin2Phi(wh) / (alphay * alphay))) /
           (M_PI * alphaX * alphay * cos4Theta);
}

double BeckmannDistribution::Lambda(const Vec3d &w, const Vec2d &alphaXY) const {
    double alphaX = alphaXY.x;
    double alphay = alphaXY.y;

    double absTanTheta = fm::abs(TanTheta(w));
    if (std::isinf(absTanTheta)) return 0.;
    double alpha =
        fm::sqrt(Cos2Phi(w) * alphaX * alphaX + Sin2Phi(w) * alphay * alphay);
    double a = 1 / (alpha * absTanTheta);
    if (a >= 1.6) return 0;
    return (1 - 1.259 * a + 0.396 * a * a) / (3.535 * a + 2.181 * a * a);
}

Vec3d BeckmannDistribution::Sample_wh(const Vec3d &wo, Point2d u, const Vec2d &alphaXY) const {
    if (CosTheta(wo) < 0) {
        return Sample_wh(-wo, u, alphaXY);
    }
    double alphaX = alphaXY.x;
    double alphay = alphaXY.y;

    // random sample  no importance sampling
    double tan2Theta;
    double phi;
    // todo add support sample visible area
    if (!sampleVisibleArea) {
        if (alphaX == alphay) {
            tan2Theta = -fm::log(1 - u[0]) * alphaX * alphaX;
            phi = u[1] * 2 * M_PI;
        } else {

            phi = fm::atan(alphay / alphaX *
                           fm::tan(2 * M_PI * u[1] + 0.5 * M_PI));
            if (u[1] > 0.5) phi += M_PI;
            double sinPhi = fm::sin(phi), cosPhi = fm::cos(phi);
            double alphaX2 = alphaX * alphaX, alphay2 = alphay * alphay;
            tan2Theta = -fm::log(1 - u[0]) /
                        (cosPhi * cosPhi / alphaX2 + sinPhi * sinPhi / alphay2);
        }
        double cosTheta = fm::sqrt(1 / (1 + tan2Theta));
        double sinTheta = fm::sqrt(1 / (1 + 1 / tan2Theta));
        Vec3d wh = Vec3d(sinTheta * fm::cos(phi), sinTheta * fm::sin(phi), cosTheta);
        return wh;
    }
    // see https://hal.inria.fr/hal-00996995v1/document // todo
    else {
    }
}

std::string BeckmannDistribution::ToString() const {
    return std::string();
}

double GGXDistribution::G(const Vec3d &wo, const Vec3d &wi, const Vec2d &alphaXY) const {
    return G1(wo, alphaXY) * G1(wi, alphaXY);
}

double GGXDistribution::roughnessToAlpha(double roughness) const {
    return roughness;
}

double GGXDistribution::D(const Vec3d &wh, const Vec2d &alphaXY) const {
    double alphaX = alphaXY.x;
    double alphaY = alphaXY.y;
    double ax2 = alphaX * alphaX;
    double ay2 = alphaY * alphaY;
    Vec3d wh2 = wh * wh;
    double D = M_PI * alphaX * alphaY * pow(wh2.x / ax2 + wh2.y / ay2 + wh2.z, 2);
    return 1 / D;
}

double GGXDistribution::Lambda(const Vec3d &w, const Vec2d &alphaXY) const {
    double ax2 = alphaXY.x * alphaXY.x;
    double ay2 = alphaXY.y * alphaXY.y;
    Vec3d v2 = w * w;
    double Lambda = (-1 + sqrt(1 + (v2.x * ax2 + v2.y * ay2) / v2.z)) / 2;
    return Lambda;
}

Vec3d GGXDistribution::Sample_wh(const Vec3d &wo, Point2d u, const Vec2d &alphaXY) const {
    u.x = std::max(0.01, std::min(0.99, u.x));
    u.y = std::max(0.01, std::min(0.99, u.y));
    double alphaX = alphaXY.x, alphaY = alphaXY.y;
    if (CosTheta(wo) < 0) {
        return Sample_wh(-wo, u, alphaXY);
    }
    if (sampleVisibleArea) {
        // see https://jcgt.org/published/0007/04/01/slides.pdf
        //  Transform the incoming direction to the "hemisphere configuration".
        Vec3d hemisphereDirOut = normalize(Vec3d(alphaX * wo.x, alphaY * wo.y, wo.z));
        // Parameterization of the projected area of a hemisphere.
        double r = sqrt(u.x);
        double phi = 2 * M_PI * u.y;
        double t1 = r * cos(phi);
        double t2 = r * sin(phi);
        // Vertically scale the position of a sample to account for the projection.
        double s = (1 + hemisphereDirOut.z) / 2;
        t2 = (1 - s) * sqrt(1 - t1 * t1) + s * t2;
        // Point in the disk space
        Vec3d diskN{t1, t2, sqrt(std::max(0.0, 1 - t1 * t1 - t2 * t2))};
        // Reprojection onto hemisphere -- we get our sampled normal in hemisphere space.
        Vec3d T1 = normalize(Vec3d(-hemisphereDirOut.y, hemisphereDirOut.x, 0));
        Vec3d T2 = cross(hemisphereDirOut, T1);
        Vec3d hemisphereN = t1 * T1 + t2 * T2 + diskN.z * hemisphereDirOut;

        // Transforming the normal back to the ellipsoid configuration
        return normalize(Vec3d(alphaX * hemisphereN.x, alphaY * hemisphereN.y, std::max(0.0, hemisphereN.z)));
    } else {
        double cosTheta, phi = (2 * M_PI) * u[1];
        if (alphaX == alphaY) {
            double tanTheta2 = alphaX * alphaY * u[0] / (1.0f - u[0]);
            cosTheta = 1 / fm::sqrt(1 + tanTheta2);
        } else {
            phi =
                fm::atan(alphaY / alphaX * fm::tan(2 * M_PI * u[1] + .5f * M_PI));
            if (u[1] > .5f) phi += M_PI;
            double sinPhi = fm::sin(phi), cosPhi = fm::cos(phi);
            const double alphaX2 = alphaX * alphaX, alphaY2 = alphaY * alphaY;
            const double alpha2 =
                1 / (cosPhi * cosPhi / alphaX2 + sinPhi * sinPhi / alphaY2);
            double tanTheta2 = alpha2 * u[0] / (1 - u[0]);
            cosTheta = 1 / fm::sqrt(1 + tanTheta2);
        }
        double sinTheta =
            fm::sqrt(std::max((double)0., (double)1. - cosTheta * cosTheta));
        Vec3d wh = Vec3d(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
        return wh;
    }
}

std::string GGXDistribution::ToString() const {
    return std::string();
}

double MicrograinDistribution::D(const Vec3d &wh, const Vec2d &alphaXY) const {
    double tau0 = alphaXY[0];
    double beta = alphaXY[1];
    tau0 = std::clamp(tau0, 0.00, 0.999);

    double beta2 = beta * beta;
    Vec3d half = normalize(wh);
    double cos = CosTheta(half);
    double cos2 = cos * cos;
    double tan2 = (1.f - cos2) / cos2;
    double tmp = beta2 + tan2;
    double num = beta2 * fm::log(1. - tau0) * fm::pow<float>(1. - tau0, tan2 / tmp);
    float denum = tau0 * fm::pi_d * tmp * tmp * cos2 * cos2;
    return -num / denum;
}

Vec3d MicrograinDistribution::Sample_wh(const Vec3d &wo, Point2d u, const Vec2d &alphaXY) const {
    double tau0 = alphaXY[0];
    double beta = alphaXY[1];
    double tmp = fm::log(1 - u[0] * tau0);
    double tan_theta_2 = beta * beta * (-tmp / (tmp - fm::log(1 - tau0)));
    double phi = u[1] * 2 * fm::pi_d;

    double cos_theta = fm::sqrt(1. / (1. + tan_theta_2));
    double sin_theta = fm::sqrt(1. - cos_theta * cos_theta);
    return {sin_theta * fm::cos(phi), cos_theta, sin_theta * fm::sin(phi)};
}

std::string MicrograinDistribution::ToString() const {
    return std::string();
}

double MicrograinDistribution::Lambda(const Vec3d &w, const Vec2d &alphaXY) const {
    // see https://dl.acm.org/doi/10.1145/3610548.3618241
    double tau0 = alphaXY[0];
    double beta = alphaXY[1];
    double pi_gamma = -log(1.f - tau0);
    double exp_pi_gamma_minus_one = exp(pi_gamma) - 1.;
    double cos = std::clamp(CosTheta(w), 0.00001, 0.99999);
    double mu = cos / sqrt(1. - cos * cos);

    double beta2 = beta * beta;
    double beta4 = beta2 * beta2;
    double beta6 = beta4 * beta2;
    double beta8 = beta6 * beta2;
    double beta10 = beta8 * beta2;
    double beta12 = beta10 * beta2;

    double mu2 = mu * mu;
    double mu4 = mu2 * mu2;
    double mu6 = mu4 * mu2;
    double mu8 = mu6 * mu2;
    double mu10 = mu8 * mu2;
    double mu12 = mu10 * mu2;

    double beta2_mu2 = beta2 + mu2;
    double sqrt_beta2_mu2 = sqrt(beta2_mu2);

    double F0 = pi_gamma * (-mu + sqrt_beta2_mu2) / (2. * mu);

    double F1 = pow(pi_gamma, 2.) * (beta2 + 2. * mu * (mu - sqrt_beta2_mu2)) / (8. * mu * sqrt_beta2_mu2);

    double F2 = pow(pi_gamma, 3.) * (3. * beta4 + 12. * beta2 * mu2 + 8. * mu4 - 8. * mu * pow(beta2_mu2, 3. / 2.)) / (96. * mu * pow(beta2_mu2, 3. / 2.));

    double F3 = pow(pi_gamma, 4.) * (5. * beta6 + 30. * beta4 * mu2 + 40. * beta2 * mu4 + 16. * mu6 - 16. * mu * pow(beta2_mu2, 5. / 2.)) / (768. * mu * pow(beta2_mu2, 5. / 2.));

    double F4 = pow(pi_gamma, 5.) * (35. * beta8 + 280. * beta6 * mu2 + 560. * beta4 * mu4 + 448. * beta2 * mu6 + 128. * mu8 - 128. * mu * pow(beta2_mu2, 7. / 2.)) / (30720. * mu * pow(beta2_mu2, 7. / 2.));

    double F5 = pow(pi_gamma, 6.) * (63. * beta10 + 630. * beta8 * mu2 + 1680. * beta6 * mu4 + 2016. * beta4 * mu6 + 1152. * beta2 * mu8 + 256. * mu10 - 256. * mu * pow(beta2_mu2, 9. / 2.)) / (368640. * mu * pow(beta2_mu2, 9. / 2.));

    double F6 = pow(pi_gamma, 7.) * (231. * beta12 + 2772. * beta10 * mu2 + 9240. * beta8 * mu4 + 14784. * beta6 * mu6 + 12672. * beta4 * mu8 + 5632. * beta2 * mu10 + 1024. * mu12 - 1024. * mu * pow(beta2_mu2, 11. / 2.)) / (10321920. * mu * pow(beta2_mu2, 11. / 2.));

    double lambda_ = (F0 + F1 + F2 + F3 + F4 + F5 + F6) / exp_pi_gamma_minus_one;

    return lambda_;
}

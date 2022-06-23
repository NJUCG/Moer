#include "CoreLayer/Geometry/Geometry.h"
#include "Common.h"

static  float TentInverse(float x){
    if(x<=.5f)
        return std::sqrt(2*x)-1;
    return  1- std::sqrt(2-2*x);
}
inline   Point2d SquareToTent(const Point2d &sample) {
    Point2d  res(TentInverse(sample[0]), TentInverse(sample[1]));
    return  res;
//    throw NoriException("SquareToTent() is not yet implemented!");
}

inline  float SquareToTentPdf(const Point2d &p) {
    return (1.f-abs(p[0])) * (1.f-abs(p[1]));
}


//Point2d SquareToUniformDisk(const Point2d &sample) {
//    auto phi=2*sample.x()*M_PI;
//    auto r=sqrt(sample.y());
//    return {r*cos(phi),r*sin(phi)};
//}
//
//float SquareToUniformDiskPdf(const Point2d &p) {
//    return  < 1.f ? INV_PI : .0f;}

inline  Vec3d SquareToUniformSphere(const Point2d &sample) {
    float z = 1 - 2 * sample[0];
    float r = std::sqrt(std::max((float )0, (float)1 - z * z));
    float phi = 2 * M_PI * sample[1];
    return {r * std::cos(phi), r * std::sin(phi), z};
}

inline  float SquareToUniformSpherePdf(const Vec3d &v) {
    return 0.25f*INV_PI;
}

inline  Vec3d SquareToUniformHemisphere(const Point2d &sample) {
    float z = 1 - 2 * sample[0];
    float r = std::sqrt(std::max((float )0, (float)1 - z * z));
    float phi = 2 * M_PI * sample[1];
    return {r * std::cos(phi), r * std::sin(phi), abs(z)};
}

inline float SquareToUniformHemispherePdf(const Vec3d &v) {
    return v[2] >=0 ? 0.5f * INV_PI : .0f;
}

inline  Vec3d SquareToCosineHemisphere(const Point2d &sample) {
    float z=sqrt(1-sample.x);
    float phi=sample.y*2*M_PI;

    return {sqrt(sample.x)* cos(phi),sqrt(sample.x)*sin(phi),z};
}

inline  float SquareToCosineHemispherePdf(const Vec3d &v) {
    return v[2] >=0 ? v.z * INV_PI : .0f;
}
//
inline  Vec3d SquareToBeckmann(const Point2d &sample,double alpha) {
    auto tan2theta= -alpha*alpha*log( sample.x );
    auto cosTheta=sqrt(1/(1+tan2theta));
    auto sinTheta= sqrt(1-cosTheta*cosTheta);
    auto phi=sample.y * 2 * M_PI;
    Vec3d t1= Vec3d(sinTheta*cos(phi), sinTheta*sin(phi),cosTheta);
    return t1;
}
//
inline  float SquareToBeckmannPdf(const Vec3d &m, double alpha) {
    if(m.z<=0)
        return 0.0f;
    auto cosTheta=m.z;
    auto sinTheta=sqrt(1-cosTheta*cosTheta);
    auto tan2Theta=(sinTheta* sinTheta)/(cosTheta*cosTheta);
    float azimuthal =   INV_PI;
    float longitudinal = exp(-tan2Theta/(alpha*alpha))  / (alpha*alpha*pow(cosTheta,3));
    return azimuthal * longitudinal;
}




#include "Microfacet.h"

Mircofacet::Mircofacet(Spectrum kd, double intIOR, double extIOR,double alpha) : kd(kd),intIOR(intIOR),extIOR(extIOR),alpha(alpha) {
    RGB3 s= kd.toXYZ3().toRGB3();
    ks =1-std::max(s[0],std::max(s[1],s[2]));



}

Spectrum Mircofacet::f(const Vec3d &wo, const Vec3d &wi) const {
    Spectrum diffuse =kd * INV_PI;

    Vec3d  wh=normalize((wi+wo));

    double F= fresnel(dot(wh,wi),extIOR,intIOR);

    auto G_1=[&](const Vec3d & wv,const Vec3d & wh){
        if(dot(wv,wh)/Frame::cosTheta(wv)<=0)
            return 0.f;
        float b=1.f/(alpha * Frame::tanTheta(wv));
        if(b>=1.6){
            return 1.f;
        }
        else
        {
            auto mol=3.535f*b+2.181f*b*b;
            auto den=1+2.276f*b+2.577f*b*b;
            return float(mol/den);
        }
    };
    auto G=G_1(wi,wh)*G_1(wo,wh);

    auto D=SquareToBeckmannPdf(wh,alpha);

    Spectrum spec=ks * D * F * G /
               (4 * Frame::cosTheta(wi) * Frame::cosTheta(wo) * Frame::cosTheta(wh));

    return diffuse + spec ;

}

Vec3d Mircofacet::sampleWi(const Vec3d &wo, const Point2d &sample) const {
    Vec3d  wi;
    if(sample.x<ks){
        ///specular case
        const Vec3d n = SquareToBeckmann(sample,alpha);
        Vec3d  inDir=-wo;
        wi = inDir-2*dot(inDir,n)*n;
    }
    else{
        ///diffuse case
        wi=SquareToCosineHemisphere(sample);
    }
    return wi;

}

double Mircofacet::pdf(const Vec3d &wo, const Vec3d &wi) const {
        if (Frame::cosTheta(wi) <= 0.0f
            || Frame::cosTheta(wo) <= 0.0f)
            return 0.0f;
        Vec3d wh = normalize(wi +wo);

        float jacobian = 1.f/(4* dot(wh,wi));
        double a = ks *SquareToBeckmannPdf(wh, alpha) * jacobian;
        double b = (1 - ks) * wi.z * INV_PI;
        return a + b;


}

BxDFSampleResult Mircofacet::sample(const Vec3d &wo, const Point2d &sample) const {
    BxDFSampleResult result ;
    result.directionIn= sampleWi(wo,sample);
    result.isSpecular=isSpecular();
    result.pdf= pdf(wo,result.directionIn);
    result.s= f(wo,result.directionIn);
    return result;
}

bool Mircofacet::isSpecular() const {
    return false;
}


#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/TestMirrorBxdf.h"



#pragma  once
class TestMirror     : public  Material{
private:
    Spectrum albedo;
public:
    TestMirror(Spectrum a = Spectrum(0.5));
    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;
    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;

};



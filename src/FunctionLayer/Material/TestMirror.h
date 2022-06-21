
#pragma once
#include "Material.h"
#include "FunctionLayer/Material/BxDF/TestMirrorBxdf.h"

class TestMirror : public Material
{
private:
public:
    TestMirror();
    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;
    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;
};

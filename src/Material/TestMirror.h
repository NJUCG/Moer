
#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/TestMirrorBxdf.h"

#pragma once
class TestMirror : public Material
{
private:
public:
    TestMirror();
    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;
    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;
};

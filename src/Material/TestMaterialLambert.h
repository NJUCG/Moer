#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/TestBxDFLambert.h"

#pragma once

class TestMaterialLambert: public Material
{
public:
    TestMaterialLambert();
    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;
    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;
};
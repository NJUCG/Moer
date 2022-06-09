#include "TestMaterialLambert.h"
#include "../Core/FunctionLayer/Intersection.h"
#include "../BxDF/TestBxDFLambert.h"
#include "../Texture/ProceduralTexture.h"

TestMaterialLambert::TestMaterialLambert(Spectrum s) : s(s)
{
}

std::shared_ptr<BxDF> TestMaterialLambert::getBxDF(Intersection intersect) const
{
    std::shared_ptr<TestBxDFLambert> bxdf = std::make_shared<TestBxDFLambert>(intersect);
    bxdf->diffuse = s;
    return bxdf;
}

std::shared_ptr<BSSRDF> TestMaterialLambert::getBSSRDF(Intersection intersect) const
{
    return nullptr;
}

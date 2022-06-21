#include "TestMirror.h"

TestMirror::TestMirror()
{
}

std::shared_ptr<BxDF> TestMirror::getBxDF(Intersection intersect) const
{
    std::shared_ptr<TestMirrorBxdf> bxdf = std::make_shared<TestMirrorBxdf>();
    return bxdf;
}
std::shared_ptr<BSSRDF> TestMirror::getBSSRDF(Intersection intersect) const
{
    return nullptr;
}
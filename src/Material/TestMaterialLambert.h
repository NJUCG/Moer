#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/TestBxDFLambert.h"

#pragma once

class TestMaterialLambert : public Material
{
  Spectrum s;

public:
  TestMaterialLambert(Spectrum s = Spectrum(0.2));
  virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;
  virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;
};
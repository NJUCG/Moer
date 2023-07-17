#include "Distribution.h"

Distribution2D::Distribution2D(const double * data, int nu, int nv) {
    pConditionalV.reserve(nv);
    for(int i=0;i<nv;i++){
        pConditionalV.emplace_back(std::make_unique<Distribution1D>(data+nu*i,nu));
    }
    std::vector<double> marginalFunc;
    marginalFunc.reserve(nv);
    for (int v = 0; v < nv; ++v)
        marginalFunc.push_back(pConditionalV[v]->funcInt);
    pMarginal.reset(new Distribution1D(&marginalFunc[0], nv));
}


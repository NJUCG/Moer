#include "Heterogeneous.h"
#include <nanovdb/util/IO.h>
HeterogeneousMedium::HeterogeneousMedium(std::shared_ptr<PhaseFunction> phase) : Medium(phase) {
    //TODO hard code
    std::string vdbfilepath = "/home/zcx/Programming/Moer/scenes/heterogeneous/models/cloud.nvdb";
    densityGrid = nanovdb::io::readGrid(vdbfilepath, "density", 1);
    densityFloatGrid = densityGrid.grid<float>();
    if (!densityGrid) {
        std::cout << ".nvdb file must contains density grid\n";
        exit(1);
    }

    temperatureGrid = nanovdb::io::readGrid(vdbfilepath, "temperature", 1);
    temperatureFloatGrid = temperatureGrid.grid<float>();

    //* Compute density grid bound
    auto bbox = densityFloatGrid->worldBBox();
    boxMin = Point3d(bbox.min()[0], bbox.min()[1], bbox.min()[2]);
    boxMax = Point3d(bbox.max()[0], bbox.max()[1], bbox.max()[2]);
}

bool HeterogeneousMedium::sampleDistance(MediumSampleRecord *mRec, const Ray &ray, const Intersection &its, Point2d sample) const {
    //
}

Spectrum HeterogeneousMedium::evalTransmittance(Point3d from, Point3d dest) const {
    //
}
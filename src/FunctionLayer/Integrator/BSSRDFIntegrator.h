#include "PathIntegrator-new.h"

struct SubSurfaceLocalRecord : PathIntegratorLocalRecord {
    Spectrum L;
    Point3d point;
};

class BSSRDFIntegrator : public PathIntegratorNew {
public:
    Spectrum Li(const Ray & ray, std::shared_ptr < Scene > scene) override;

    BSSRDFIntegrator(std::shared_ptr<Camera> _camera,
                     std::unique_ptr<Film> _film,
                     std::unique_ptr<TileGenerator> _tileGenerator,
                     std::shared_ptr<Sampler> _sampler,
                     int _spp,
                     int _renderThreadNum = 4);

    SubSurfaceLocalRecord
    sampleSubsurface(std::shared_ptr < Scene > scene, const Intersection & its, const Ray & tempray, const Spectrum & throughput);


    PathIntegratorLocalRecord
    sampleScatter(std::shared_ptr < Scene > scene, const Intersection & its, const Ray & ray) ;
};
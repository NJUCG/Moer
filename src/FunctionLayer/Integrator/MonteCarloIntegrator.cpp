/**
 * @file MonteCarloIntegrator.h
 * @author Zhimin Fan
 * edited by orbitchen at 2022-7-7: apply multithread acceleration and tile generator.
 * @brief Integrators
 * @version 0.1
 * @date 2022-05-06
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "MonteCarloIntegrator.h"

#include <thread>

MonteCarloIntegrator::MonteCarloIntegrator(
        std::shared_ptr<Camera> _camera, 
        std::unique_ptr<Film> _film, 
        std::unique_ptr<TileGenerator> _tileGenerator, 
        std::shared_ptr<Sampler> _sampler, 
        int _spp,
        int _renderThreadNum):
            Integrator(_camera,std::move(_film)),
            tileGenerator(std::move(_tileGenerator)),
            sampler(_sampler),
            spp(_spp),
            renderThreadNum(_renderThreadNum)
{

}

void MonteCarloIntegrator::renderPerThread(std::shared_ptr<Scene> scene)
{
    while(true)
    {
        auto optionalTile=tileGenerator->generateNextTile();
        if(optionalTile==std::nullopt)
            break;
        auto tile=optionalTile.value();

        for(auto it=tile->begin();it!=tile->end();it++)
        {
            auto pixelPosition=*it;

            const auto &cam = *this->camera;
            sampler->startPixel(pixelPosition);
            for (int i = 0; i < spp; i++)
            {
                auto L = Li(
                    cam.generateRay(
                        film->getResolution(), 
                        pixelPosition, 
                        sampler->getCameraSample()
                    ), scene
                );
                film->deposit(pixelPosition, L);
                sampler->nextSample();
            }
        }
    }
    
}

void MonteCarloIntegrator::render(std::shared_ptr<Scene> scene)
{
    std::vector<std::thread> threads;
    for(int i=0;i<renderThreadNum;i++){
        threads.push_back(std::thread(&MonteCarloIntegrator::renderPerThread,this,scene));
    }

    for(int i=0;i<renderThreadNum;i++){
        threads[i].join();
    }
    // int filmWidth = film->getResolution().x;
    // int filmHeight = film->getResolution().y;
    // const auto &cam = *this->camera;
    // for (int y = 0; y < filmHeight; y++)
    // {
    //     for (int x = 0; x < filmWidth; x++)
    //     {
    //         Point2i pixelPosition = Point2i{x, y};
    //         sampler->startPixel(pixelPosition);
    //         for (int i = 0; i < spp; i++)
    //         {
    //             auto L = Li(
    //                 cam.generateRay(
    //                     film->getResolution(), 
    //                     pixelPosition, 
    //                     sampler->getCameraSample()
    //                 ), scene
    //             );
    //             film->deposit(Point2d(x, y), L);
    //             sampler->nextSample();
    //         }
    //     }
    // }
}

double MonteCarloIntegrator::randFloat()
{
    // Get a random number WITHOUT using MonteCarloIntegrator::sampler
    return rand() * 1.0 / RAND_MAX; // todo: better solution
}

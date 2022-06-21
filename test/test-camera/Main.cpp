#include "FunctionLayer/Camera/Perspective.h"
#include "FunctionLayer/Camera/Pinhole.h"
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

void printMatrix (const Matrix4x4 &mat) {
    std::cout << mat.matrix << std::endl;
}

TEST_CASE("test-camera") {
    /*
    !All passed

    PerspectiveCamera *camera = 
        new PerspectiveCamera (
            Point3d (1, 2, 0),
            Point3d (1, 2, 1),
            Vec3d (0, 1, 0),
            90,
            1.7778,
            1.7778
        );
    
    REQUIRE(camera->cameraToWorld * Point3d(0, 0, 0) == Point3d(1, 2, 0));
    REQUIRE(camera->cameraToWorld * Point3d(0, 0, 1) == Point3d(1, 2, 1));
    REQUIRE(camera->cameraToWorld * Point3d(1, 0, 1) == Point3d(2, 2, 1));

    REQUIRE(camera->cameraToWorld * Vec3d(0, 0, 1) == Vec3d(0, 0, 1));
    REQUIRE(camera->cameraToWorld * Vec3d(0, 1, 0) == Vec3d(0, 1, 0));
    REQUIRE(camera->cameraToWorld * Vec3d(1, 0, 1) == Vec3d(1, 0, 1));

    std::cout << camera->sampleToFilm * Point3d(0, 0, 0) << std::endl;      // Point3d(-1.7778, 1, 1.7778));
    std::cout << camera->sampleToFilm * Point3d(1, 1, 0) << std::endl;      // Point3d(1.7778, -1, 1.7778));
    std::cout << camera->sampleToFilm * Point3d(0.5, 0.5, 0) << std::endl;  // Point3d(0, 0, 1.7778));
    */

    PinholeCamera *camera = 
        new PinholeCamera (
            Point3d (0, 1, 1),
            Point3d (0, 0, 0),
            Vec3d (0, 1, 0),
            90,
            1,
            1
        );
//    printMatrix(camera->cameraToWorld);
    Ray ray = camera->generateRay(Point2d(0.5, 0.5));
    std::cout << "Origin     = " << ray.origin << std::endl;
    std::cout << "direction  = " << ray.direction << std::endl;

}
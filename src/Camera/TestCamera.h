/**
 * @file TestCamera.h
 * @author Zhimin Fan
 * @brief A fixed camera only for unit test of integrator
 * @version 0.1
 * @date 2022-05-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../Core/FunctionLayer/Camera.h"

// @brief This camera always generates rays along direction (0,0,-1), started at (x,y,INF) where (x,y) is specified by NDC.
class TestCamera : public Camera
{
public:
    // @brief generate a ray by NDC coord ranges between [-1,1].
    Ray generateRay(Point2d NDC);
};
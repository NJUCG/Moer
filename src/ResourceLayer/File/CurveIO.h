/**
* @file CurveIO.h
* @author JunPing Yuan
* @brief IO Of Curve
* @version 0.1
* @date 2023/6/13
*
* @copyright Copyright (c) 2022
*
*/

#pragma once
#include "CoreLayer/Geometry/Geometry.h"
#include <vector>
namespace CurveIO{
    void LoadCurve(const std::string & path, std::vector < int > * curveEnds, std::vector < float > *  nodeData,
                   std::vector <Vec3f> * nodeColor, std::vector <Vec3f> * nodeNormals);
    void LoadCurve(const std::string & path, std::vector < int > * curveEnds, std::vector < double > *  nodeData,
                            std::vector <Vec3d> * nodeColor, std::vector <Vec3d> * nodeNormals);
}
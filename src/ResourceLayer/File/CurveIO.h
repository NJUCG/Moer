#include "CoreLayer/Geometry/Geometry.h"
#include <vector>
namespace CurveIO{
    void LoadCurve(const std::string & path, std::vector < int > * curveEnds, std::vector < float > *  nodeData,
                   std::vector <Vec3f> * nodeColor, std::vector <Vec3f> * nodeNormals);
    void LoadCurve(const std::string & path, std::vector < int > * curveEnds, std::vector < double > *  nodeData,
                            std::vector <Vec3d> * nodeColor, std::vector <Vec3d> * nodeNormals);
}
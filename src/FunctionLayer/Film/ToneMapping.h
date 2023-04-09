/**
 * @file ToneMapping.h
 * @author JunPing Yuan
 * @brief tone mapping
 * @version 0.1
 * @date 2022-09-20
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "CoreLayer/ColorSpace/Color.h"
#include "CoreLayer/Geometry/Matrix.h"
#include "FastMath.h"

static inline  RGB3 filmicACES(const RGB3 &in)
{
    Eigen::Matrix3d ACESInputMat;
    ACESInputMat << 0.59719, 0.07600, 0.02840,
                    0.35458, 0.90834, 0.13383,
                    0.04823, 0.01566, 0.83777;


    Eigen::Matrix3d ACESOutputMat;
    ACESInputMat << 1.60475, -0.10208, -0.00327,
                    -0.53108, 1.10813, -0.07276,
                    -0.07367, -0.00605, 1.07602;

    Eigen::Vector3d colorVec(in[0],in[1],in[2]);

    auto RRTAndODTFit = [](const Eigen::Vector3d &v)
    {
        Vec3d tempV(v.x(),v.y(),v.z());
        Vec3d a = tempV * ( tempV + 0.0245786) - 0.000090537;
        Vec3d b = tempV * (0.983729 * tempV + 0.4329510) + 0.238081;
        Vec3d c = a / b;
        return Eigen::Vector3d(c.x,c.y,c.z);
    };

    Eigen::Vector3d color = ACESOutputMat * RRTAndODTFit(ACESInputMat * colorVec);

    for(int i=0;i<3;i++)
        color[i] = std::clamp(color[i],0.0,1.0);
    return RGB3(color.x(),color.y(),color.z());
}

class ToneMapping{
public:
    //  friend Type;
    enum ToneMapType {
        LinearOnly,
        GammaOnly,
        Reinhard,
        Filmic,
        Pbrt,
        Aces
    };

    static inline RGB3 toneMap(ToneMapType type, const RGB3 &c)
    {
        RGB3 color = c;
        switch (type) {

            case LinearOnly:
                return c;
            case GammaOnly:
                return color.pow(double(1/2.2));
            case Reinhard:
                return (c/(c + RGB3(1.0))).pow(1.0/2.2);
            case Filmic: {
                RGB3 x;
                for(int i=0;i<3;i++)
                x[i] = std::max(0.0, c[i] - 0.004);
                return (x*(6.2*x + 0.5))/(x*(6.2*x + 1.7) + 0.06);
            } case Pbrt: {
                RGB3  result;
                for (int i = 0; i < 3; ++i) {
                    if (c[i] < 0.0031308)
                        result[i] = 12.92*c[i];
                    else
                        result[i] = 1.055*fm::pow(c[i], 1.0f/2.4) - 0.055;
                }
                return result;
            } case Aces:{
                return filmicACES(c);
            }
        }

        return c;
    }
};
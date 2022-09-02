/**
 * @file CoordConvertor.h
 * @author Zhimin Fan
 * @brief Provide convertor between standard coordinates.
 * @version 0.1
 * @date 2022-05-27
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "Geometry.h"

/// \ingroup Geometry
/// \brief Provides transformations between different coordinate systems
/// \todo A namespace contains function might better?
class CoordConvertor
{
public:
    /// \brief Convert a spherical coordinate \f$ (\phi, \theta) \f$ to cartesian coordinate
    /// \todo Sperical2cartesian? The function name doesn't match
    /// \param p \f$ (\phi, \theta) \f$
    /// \return The correspoding coordinate in cartesian
    static Point3d cartesian2Spherical(const Point2d &p);

    static Vec3d cartesian2SphericalVec(const Point2d &p);
    
    static Vec3d world2TBN(const Vec3d &v, const Vec3d &t, const Vec3d &b, const Vec3d &n);
    
    static Vec3d TBN2World(const Vec3d &v, const Vec3d &t, const Vec3d &b, const Vec3d &n);
};
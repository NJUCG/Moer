/**
 * @file Intersection.h
 * @author orbitchen
 * @brief Light representation.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/ColorSpace/Color.h"
#include "CoreLayer/Ray/Ray.h"
#include "FunctionLayer/Intersection.h"
#include "FunctionLayer/Medium/Medium.h"

struct LightSampleResult
{
	/// @brief Spectrum from light.
	Spectrum s;

	/// @brief Point on an object where receives light if exists.
	Point3d src;

	/// @brief Point on a light where emits light if exists.
	Point3d dst;

	/// @brief Direction from src to dst.
	Vec3d wi;

	Normal3d dstNormal;
	Point2d uv;

	/// @brief PDF of direct light sampling.
	double pdfDirect;
	/// @brief Positional emission PDF if exists.
	double pdfEmitPos;
	/// @brief Directional emission PDF if exists.
	double pdfEmitDir;

	/// @brief FALSE for area and volume light, TRUE for point and etc
	bool isDeltaPos;
	/// @brief TRUE for distant light
	bool isDeltaDir;
};

enum class ELightType{
    UNDEFINED,
    POINT,
    AREA,
// Windows SDK has a definition of INFINITE
#undef INFINITE
    INFINITE
};

/// \defgroup Light

/// \brief The base class for all light source
class Light
{
public:
	// @brief Assume that the given ray hits nothing in the scene.
	// Note that this function will not return a direct light sampling PDF.
	virtual LightSampleResult evalEnvironment(const Ray &ray) = 0;
	// @brief Assume that the given intersection is on the emitter. 
	// Note that param ray is only used to fill 'src' field.
	virtual LightSampleResult eval(const Ray& ray, const Intersection &its, const Vec3d &d) = 0;
	// @brief Note that this function will not return a direct light sampling PDF.
	virtual LightSampleResult sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time) = 0;
	virtual LightSampleResult sampleDirect(const Intersection &its, const Point2d &sample, float time) = 0;

	virtual LightSampleResult sampleDirect(const MediumSampleRecord &mRec,
										   Point2d sample,
                                           double time) = 0;

    Light(ELightType _lightType):lightType(_lightType){}

    ELightType lightType;
};
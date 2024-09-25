#include "SdfFunctions.h"
#include "Eigen/Dense"
#include "FastMath/FastMath.h"
#include "CoreLayer/Math/Common.h"
/*
   Copyright 2020 Towaki Takikawa @yongyuanxi
   The MIT License
   Link: N/A
   */

/******************************************************************************
 * The MIT License (MIT)
 * Copyright (c) 2021, NVIDIA CORPORATION.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// distance functions
// taken from https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

double sdSphere(Eigen::Vector3d v, double r) {
    return v.norm() - r;
}

double sdTorus(Eigen::Vector3d p, Eigen::Vector2d t) {
    Eigen::Vector2d q = Eigen::Vector2d(fm::sqrt(p.x() * p.x() + p.z() * p.z()) - t.x(), p.y());
    return q.norm() - t.y();
}

double sdCone(Eigen::Vector3d p, Eigen::Vector2d c) {
    double q = (p.head<2>().norm());
    return c.dot(Eigen::Vector2d(q, p.z()));
}

double sdCappedCylinder(Eigen::Vector3d p, double h, double r) {
    Eigen::Vector2d d = (Eigen::Vector2d(p.head<2>().norm(), p.y()).cwiseAbs() - Eigen::Vector2d(h, r));
    return std::min(std::max(d.x(), d.y()), 0.) + d.cwiseMax(Eigen::Vector2d(0.,0.)).norm();
}

double sdTriPrism(Eigen::Vector3d p, Eigen::Vector2d h) {
    Eigen::Vector3d q = p.cwiseAbs();
    return std::max(q.z() - h.y(), std::max(q.x() * 0.866025 + p.y() * 0.5, -p.y()) - h.x() * 0.5);
}

double opSmoothUnion(double d1, double d2, double k) {
    double h = std::clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return lerp(d2, d1, h) - k * h * (1.0 - h);
}

double ssub(double d1, double d2, double k) {
    double h = std::clamp(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
    return lerp(d2, -d1, h) + k * h * (1.0 - h);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actual distance functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

double sdBase(Eigen::Vector3d p) {

    double base = opSmoothUnion(sdCone((Eigen::AngleAxisd(-90. * fm::pi_d / 180.0, Eigen::Vector3d(1.0, 0.0, 0.0))).toRotationMatrix() * (p + Eigen::Vector3d(0.0, 0.9, 0.0)),
                                       Eigen::Vector2d(fm::pi_d / 3.0, fm::pi_d / 3.0)),
                                sdCone((Eigen::AngleAxisd(90. * fm::pi_d / 180.0, Eigen::Vector3d(1.0, 0.0, 0.0))).toRotationMatrix() * (p - Eigen::Vector3d(0.0, 0.9, 0.0)),
                                       Eigen::Vector2d(fm::pi_d / 3.0, fm::pi_d / 3.0)),
                                0.02);
    base = std::max(base, sdCappedCylinder(p, 1.1, 0.25)) * 0.7;
    base = std::max(-sdCappedCylinder(p, 0.6, 0.3), base);
    base = std::max(-sdTriPrism((Eigen::AngleAxisd(90. * fm::pi_d / 180.0, Eigen::Vector3d(1.0, 0.0, 0.0))).toRotationMatrix() * (p + Eigen::Vector3d(0.0, 0.0, -1.0)), Eigen::Vector2d(1.2, 0.3)), base);
    return base;
}

double sdKnob(Eigen::Vector3d p) {
    double sphere = sdSphere(p, 1.0);
    double cutout = sdSphere(p - Eigen::Vector3d(0.0, 0.5, 0.5), 0.7);
    double cutout_etch = sdTorus((Eigen::AngleAxisd(-45 * fm::pi_d / 180.0, Eigen::Vector3d(1.0, 0.0, 0.0))).toRotationMatrix() * (p - Eigen::Vector3d(0.0, 0.2, 0.2)), Eigen::Vector2d(1.0, 0.05));
    double innersphere = sdSphere(p - Eigen::Vector3d(0.0, 0.0, 0.0), 0.75);

    double d = ssub(cutout, sphere, 0.1);
    d = std::min(d, innersphere);
    d = std::max(-cutout_etch, d);
    d = std::min(ssub(sphere, sdBase(p - Eigen::Vector3d(0.0, -0.775, 0.0)), 0.1), d);
    return d;
}

double sdKnobInner(Eigen::Vector3d p) {
    return sdSphere(p - Eigen::Vector3d(0.0, 0.0, 0.0), 0.75);
}

double sdKnobOuter(Eigen::Vector3d p) {
    double sphere = sdSphere(p, 1.0);
    double cutout = sdSphere(p - Eigen::Vector3d(0.0, 0.5, 0.5), 0.7);
    double cutout_etch = sdTorus((Eigen::AngleAxisd(-45. * fm::pi_d / 180.0, Eigen::Vector3d(1.0, 0.0, 0.0))).toRotationMatrix() * (p - Eigen::Vector3d(0.0, 0.2, 0.2)), Eigen::Vector2d(1.0, 0.05));
    double innersphere = sdSphere(p - Eigen::Vector3d(0.0, 0.0, 0.0), 0.75);

    double d = ssub(cutout, sphere, 0.1);
    d = std::max(d, -innersphere);
    d = std::max(-cutout_etch, d);
    d = std::min(ssub(sphere, sdBase(p - Eigen::Vector3d(0.0, -0.775, 0.0)), 0.1), d);
    return d;
}

double SdfFunctions::knob(Point3d p) {
    const double scale = 0.8;
    p *= 1.0 / scale;
    return sdKnob({p.x, p.y, p.z}) * scale;
}

double SdfFunctions::knob_inner(Point3d p) {
    const double scale = 0.8;
    p *= 1.0 / scale;
    return sdKnobInner({p.x, p.y, p.z}) * scale;
}

double SdfFunctions::knob_outer(Point3d p) {
    const float scale = 0.8;
    p *= 1. / scale;
    return sdKnobOuter({p.x, p.y, p.z}) * scale;
}

double SdfFunctions::two_spheres(Point3d p) {
    Eigen::Vector3d eigen_p = {p.x, p.y, p.z};
    return std::min((eigen_p - Eigen::Vector3d(0., 10., 0.)).norm() - 9.5, (eigen_p - Eigen::Vector3d(0., -10., 0.)).norm() - 9.5);
}
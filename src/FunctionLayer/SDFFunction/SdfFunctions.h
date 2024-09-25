#pragma once
#include "CoreLayer/Geometry/Geometry.h"
class SdfFunctions {
public:
    enum class Function {
        Knob,
        KnobInner,
        KnobOuter,
        TwoSpheres
    };

    static std::string funcEnumToString(Function func) {
        switch (func) {
            case Function::Knob:
                return "knob";
            case Function::KnobInner:
                return "knob_inner";
            case Function::KnobOuter:
                return "knob_outer";
            case Function::TwoSpheres:
                return "two_spheres";
            default:
                return "";
        }
    }
    static Function funcStringToEnum(const std::string &func) {
        if (func == "knob") {
            return Function::Knob;
        } else if (func == "knob_inner") {
            return Function::KnobInner;
        } else if (func == "knob_outer") {
            return Function::KnobOuter;
        } else if (func == "two_spheres") {
            return Function::TwoSpheres;
        }
        return Function::Knob;
    }

    static double knob(Point3d p);
    static double knob_inner(Point3d p);
    static double knob_outer(Point3d p);
    static double two_spheres(Point3d p);

    static double eval(Function fn, Point3d p) {
        switch (fn) {
            case Function::Knob: return knob(p);
            case Function::KnobInner: return knob_inner(p);
            case Function::KnobOuter: return knob_outer(p);
            case Function::TwoSpheres: return two_spheres(p);
        }
    }

    /*template<typename sdf>
    static Vec3d grad(sdf func, Point3d p) {
        constexpr double eps = 0.001;

        std::array<double, 4> vals = {
            func(p + Vec3d(eps, 0., 0.)),
            func(p + Vec3d(0., eps, 0.)),
            func(p + Vec3d(0., 0., eps)),
            func(p)};

        return Vec3d(
                   vals[0] - vals[3],
                   vals[1] - vals[3],
                   vals[2] - vals[3]) /
               eps;
    }
    static Vec3d grad(Function fn, Point3d p) {
        switch (fn) {
            case Function::Knob: return grad(knob, p);
            case Function::KnobInner: return grad(knob_inner, p);
            case Function::KnobOuter: return grad(knob_outer, p);
            case Function::TwoSpheres: return grad(two_spheres, p);
        }
    }*/
};
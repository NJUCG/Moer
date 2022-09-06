---
author: "fzs"

---

Some note to understand the project.

# problems or questions

1. cmake configure error of assimp? UNKNOWN_PLATFORM
2. CoreLayer/Adapter/Attribute.h has some macro expanding error, relating to Win SDK and UNICODE
3. see no need to include vector.h in CoreLayer/Geometry/point.h, maybe for future coding?
4. BoundingBox3 cstr sanity check?
5. What is Tent? (CoreLayer/Math/Warp.h)
6. Ray is a structure?

# code

## CoreLayer

### Adapter

For adapting with third-party libraries

#### Attribute

Some json parsing? found macro expanding error.

#### Random

Warp the pcg library and provide easy random numbers ([0, 1) double and [min, max) integer)

### ColorSpace

RGB3, XYZ3(CIE 1931) and Spectrum color spaces

### Geometry

#### vector

Template vector 2D and 3D

#### point

Template point 2D and 3D, with matrix-like operators (why just matrix?)

No need to include vector.h?

#### BoundingBox

I understand what the BoundingBox3::Intersection() returns, but it seems tricky.

Duplicate comment at BoundingBox.h line 12.

Name of BoundingBox*Union*Intersect(). What if it returns empty? BoundingBox3(TPoint3, TPoint3) may need sanity check.

#### normal

Incomplete.

#### Matrix

Some translation matrices and operations.

#### Geometry

Typedef and alias of vector, point and normal. Rely on Normal3d in [normal](#normal).

#### Transform3d

Incomplete.

#### CoordConvertor

Conversion between $[0, 1]^2$ and unit sphere, world coordinate and local TBN. Need detailed comments in file.

base of world coordinate: (1, 0, 0), (0, 1, 0), (0, 0, 1)

base of TBN: t, b, n

world to TBN: $(\vec{t}, \vec{b}, \vec{n})\cdot \vec{v}$, where $\vec{v}$ is based on world.

TBN to world: $(\vec{t}, \vec{b}, \vec{n})^T\cdot \vec{v}$, where $\vec{v}$ is based on TBN.

#### Frame

`coordinateSystem()` is to build a set of orthonormal base with `const Normal3d &a` unchanged.

`Normal3d` has the cstr on `Vec3d`, some code may be redundant.

`Frame` provides some coordinate operations like conversion. Is it more for TBN space?

### Math

#### Common

Can two Fresnel functions be merged?

#### Warp

What is Tent?

Some sphere sample methods and their pdf value. A bit of redundant code with [CoordConvertor](#coordconvertor).

### Ray

Why structure?

Incomplete.

### Scene

Scene::intersect() may need a time range if we want participating medium. And this class mixes BVH and entity list?

Scene::addEntity() and Scene::addLight() will change the BVH?

Incomplete, need definition for intersectionTest().


## FunctionLayer

### Shape



### Aggregate

#### BVH


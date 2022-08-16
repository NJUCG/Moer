---
author: "fzs"

---

# problems or questions

1. cmake configure error of assimp? UNKNOW_PLATFORM
2. CoreLayer/Adapter/Attribute.h has some macro expanding error, relating to Win SDK and UNICODE
3. see no need to include vector.h in CoreLayer/Geometry/point.h, maybe for future coding?
4. 

# code

## CoreLayer

### Adapter

for adapting with third-party libraries

#### Attribute

some json parsing? found macro expanding error.

#### Random

warp the pcg library and provide easy random numbers ([0, 1) double and [min, max) integer)

### ColorSpace

RGB3, XYZ3(CIE 1931) and Spectrum color spaces

### Geometry

#### vector

template vector 2D and 3D

#### point

template point 2D and 3D, with matrix-like operators (why just matrix?)

no need to include vector.h?

#### BoundingBox

I understand what the BoundingBox3.Intersection() returns, but it seems tricky.


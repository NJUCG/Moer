# Moer: Research Oriented Physically Based Renderer

![build](https://github.com/NJUCG/Moer/actions/workflows/cmake.yml/badge.svg)

Moer is a phycially based renderer, providing an "easy to deploy and develop" rendering platform for computer vision (CV) and computer graphics (CG) research.

Moer is developed by NJUMeta (www.njumeta.com) from Nanjing University.

## Features

- Easy to deploy, "install & clone & make & run"
- Structured scene description using `json`
- Interface for integrator/medium/bxdf/texture... and some pre-set implementations
- Unworried resource management for heavy resource (image/mesh)

<img width="904" alt="MoerRenderer" src="https://github.com/user-attachments/assets/071dfde0-4387-4616-a5db-f8a126f336d9">


## Gallery

![classroom](https://user-images.githubusercontent.com/46410388/204492936-a7fc480b-1557-44e9-afaf-56d0ff1537a2.jpg)
![hair](https://yjpp.oss-cn-hangzhou.aliyuncs.com/uPic/C6966B38E3AD16F8AC9CC9D86C73921E.jpg)
![teapot](https://yjpp.oss-cn-hangzhou.aliyuncs.com/uPic/tea-pot.png)
![disney](https://yjpp.oss-cn-hangzhou.aliyuncs.com/uPic/disneybsdf.png)
![micrograin-teapot](https://github.com/Cchen-77/images/blob/main/micrograin.jpg)
![gpis](https://github.com/Cchen-77/images/blob/main/gpis.jpg)
## Checklist

### Accelerator
- [X] BVH
- [X] Embree
- [ ] others

### Camera
- [x] pinhole
- [x] thinlens
- [ ] spherical
- [ ] others

### Integrator
- [x] path-tracing
- [X] volpath-tracing
- [ ] bdpt
- [ ] mlt
- [ ] photon-mapping
- [ ] others

### Sampler
- [X] independent 
- [X] stratified 
- [ ] low-discrepancy
  - [X] Halton
  - [ ] (0-2)-Sequence
  - [ ] Maximized minimal distance
  - [ ] Sobol'
- [ ] others

### Emitter
- [X] point
- [X] area
- [X] infinite
- [X] infinite cap
- [ ] others

### Media
- [X] homogeneous
- [X] hetergeneous
- [X] gaussian process implicit surface
- [ ] others

### BxDF
- [X] lambert
- [X] mirror
- [X] dielectric
- [X] conductor
- [X] disney
- [X] plastic
- [X] hair
- [X] micrograin for porous layer  
- [ ] others

### Shape
- [X] sphere
- [X] quad
- [X] triangle-mesh
- [X] curve
- [ ] others



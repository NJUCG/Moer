# Moer: Research Oriented Physically Based Renderer

Moer is a phycially based renderer, providing an "easy to deploy and develop" rendering platform for computer vision (CV) and computer graphcis (CG) research.

Moer is developed by NJUMeta (www.njumeta.com) from Nanjing University.

## Features

- Easy to deploy, "install & clone & make & run"
- Structured scene description using `json`
- Interface for integrator/medium/bxdf/texture... and some pre-set implementions
- Unworried resource management for heavy resource (image/mesh)

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
- [ ] stratified 
- [ ] low-discrepancy
- [ ] others

### Emitter
- [X] point
- [X] area
- [ ] others

### Media
- [X] homogeneous
- [ ] hetergeneous
- [ ] others

### BxDF
- [X] lambert
- [X] mirror
- [X] dielectric
- [ ] microfacet
- [ ] others

### Shape
- [X] sphere
- [X] quad
- [X] triangle-mesh
- [ ] others



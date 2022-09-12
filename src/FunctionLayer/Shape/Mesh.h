/**
 * @file Mesh.h
 * @author Chenxi Zhou
 * @brief Triangle mesh only
 * @version 0.1
 * @date 2022-08-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "Entity.h"
#include "Triangle.h"
#include "ResourceLayer/File/MeshData.h"
#include "FunctionLayer/Aggregate/Bvh.h"
#include <optional>
#include <memory>

//! Only triangle mesh
class Mesh : public Entity 
{
public:
    
    Mesh() = default;
    
    Mesh(std::shared_ptr<MeshData> _data, std::shared_ptr<Material> _material);
    
    virtual ~Mesh() = default;
    
    virtual std::optional<Intersection> intersect(const Ray &r) const override;
    
    virtual double area() const override;
    
    //TODO, name -> sampleOnSurface, param-> Point2d sample
    
    virtual Intersection sample(const Point2d &positionSample) const override;
    
    virtual std::shared_ptr<Light> getLight() const override;
    
    virtual void setLight(std::shared_ptr<Light> light) override;
    
    //TODO, why up case?
    
    virtual BoundingBox3f WorldBound() const override;

protected:
    std::shared_ptr<MeshData> data;
    std::shared_ptr<Bvh>      BVH;      ///< Spacial accelerate structure
                                        /// \todo Replace with abstruct class
    //* Make it compatible with the bvh
    std::vector<std::shared_ptr<Entity>> m_triangles;
    
    virtual void apply() override;
};
#include "Texture.h"

template <typename Tvalue>
ConstantTexture<Tvalue>::ConstantTexture(const Tvalue &value) : value(value)
{
}

template <typename Tvalue>
Tvalue ConstantTexture<Tvalue>::eval(const Intersection &intersection) const
{
    return value;
}

template <typename Tvalue>
MixTexture<Tvalue>::MixTexture(std::shared_ptr<Texture<Tvalue>> srcA,
                               std::shared_ptr<Texture<Tvalue>> srcB,
                               std::shared_ptr<Texture<float>> factor) : srcA(srcA), srcB(srcB), factor(factor)
{
}

template <typename Tvalue>
Tvalue MixTexture<Tvalue>::eval(const Intersection &intersection) const
{
    float alpha = factor->eval(intersection);
    return srcA->eval(intersection) * alpha + srcB->eval(intersection) * (1 - alpha);
}

template <typename Tvalue, typename Tcoord>
StdTexture<Tvalue, Tcoord>::StdTexture(std::shared_ptr<TextureFunction<Tvalue, Tcoord>> func,
                                       std::shared_ptr<TextureMapping<Tcoord>> map) : map(map), func(func)
{
}

template <typename Tvalue, typename Tcoord>
StdTexture<Tvalue, Tcoord>::StdTexture(std::shared_ptr<TextureFunction<Tvalue, Tcoord>> func) : func(func)
{
    // todo: use default mapping for 2D and 3D
}

template <typename Tvalue, typename Tcoord>
Tvalue StdTexture<Tvalue, Tcoord>::eval(const Intersection &intersection) const
{
    Tcoord coord = map->mapping(intersection);
    return func->eval(coord);
}
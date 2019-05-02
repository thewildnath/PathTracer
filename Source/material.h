#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "light.h"
#include "texture.h"
#include "vector_type.h"

#include <memory>

namespace scg
{

class Material
{
public:
    virtual Vec3f evaluate(Vec2f const& uv) const = 0;
    virtual void sample(SurfaceInteraction &interaction, Sampler &sampler) const = 0;
    virtual float pdf(SurfaceInteraction const& interaction) const = 0;

    virtual std::shared_ptr<Light> getLight(Vec2f const& uv) const
    {
        return nullptr;
    }
};

class Lambert : public Material
{
protected:
    Lambert() = default;

public:
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Light> light;

    Lambert(std::shared_ptr<Texture> const& texture):
        texture(texture) {};

    Lambert(std::shared_ptr<Texture> const& texture, std::shared_ptr<Light> const& light):
        texture(texture), light(light) {};

    Vec3f evaluate(Vec2f const& uv) const override
    {
        return texture->evaluate(uv);
    }

    void sample(SurfaceInteraction &interaction, Sampler &sampler) const override
    {

    }

    float pdf(SurfaceInteraction const& interaction) const override
    {

    }

    std::shared_ptr<Light> getLight(Vec2f const&) const override
    {
        return light;
    }
};

}

#endif //RAYTRACER_MATERIAL_H

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "bsdf_lobe.h"
#include "light.h"
#include "math_vector_utils.h"
#include "sampler.h"
#include "texture.h"
#include "vector_type.h"

#include <memory>

namespace scg
{

class Material
{
public:
    virtual Vec3f evaluate(SurfaceInteraction const& interaction) const = 0;
    virtual void sample(SurfaceInteraction &interaction, Sampler &sampler) const = 0;
    virtual float pdf(SurfaceInteraction const& interaction) const = 0;

    virtual BSDFLobe getSupportedLobes(Vec2f const&) const = 0;
    virtual std::shared_ptr<Light> getLight(Vec2f const&) const
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

    Vec3f evaluate(SurfaceInteraction const& interaction) const override
    {
        return texture->evaluate(interaction.uv) * std::max(0.0f, dot(interaction.normal, interaction.inputDir)) * (float)M_1_PI;
    }

    void sample(SurfaceInteraction &interaction, Sampler &sampler) const override
    {
        interaction.inputDir = SampleHemisphere(interaction.normal, sampler);
        interaction.sampledLobe = BSDFLobe::Diffuse;
    }

    float pdf(SurfaceInteraction const& interaction) const override
    {
        return std::max(0.0f, dot(interaction.normal, interaction.inputDir)) * (float)M_1_PI;
    }

    BSDFLobe getSupportedLobes(Vec2f const&) const override
    {
        return BSDFLobe::Diffuse;
    }

    std::shared_ptr<Light> getLight(Vec2f const&) const override
    {
        return light;
    }
};

class Mirror : public Material
{
protected:
    Mirror() = default;

public:
    std::shared_ptr<Texture> texture;

    Mirror(std::shared_ptr<Texture> const& texture):
        texture(texture) {};

    Vec3f evaluate(SurfaceInteraction const& interaction) const override
    {
        return texture->evaluate(interaction.uv);
    }

    void sample(SurfaceInteraction &interaction, Sampler &) const override
    {
        interaction.inputDir = reflect(interaction.outputDir, interaction.normal);
        interaction.sampledLobe = BSDFLobe::SpecularReflection;
    }

    float pdf(SurfaceInteraction const&) const override
    {
        return 1.0f;
    }

    BSDFLobe getSupportedLobes(Vec2f const&) const override
    {
        return BSDFLobe::SpecularReflection;
    }
};

}

#endif //RAYTRACER_MATERIAL_H

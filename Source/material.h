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

class SpecularDielectric : public Material
{
protected:
    SpecularDielectric() = default;

public:
    std::shared_ptr<Texture> texture;
    float ior = 1.0f;

    SpecularDielectric(std::shared_ptr<Texture> const& texture, float ior):
        texture(texture), ior(ior) {};

    Vec3f evaluate(SurfaceInteraction const& interaction) const override
    {
        return texture->evaluate(interaction.uv);
    }

    void sample(SurfaceInteraction &interaction, Sampler &sampler) const override
    {
        float VdotN = dot(interaction.outputDir, interaction.normal);
        float iorO = ior;

        // Check for side
        if (VdotN < 0.0f)
        {
            iorO = 1.0f;
            interaction.normal = -interaction.normal;
            VdotN = -VdotN;
        }

        float eta = interaction.iorI / iorO;
        float sinSquaredThetaT = SinSquaredThetaT(VdotN, eta);
        float fresnel = Fresnel(interaction.iorI, iorO, VdotN, sinSquaredThetaT);

        float rand = sampler.nextFloat();
        if (rand <= fresnel)
        {
            // Reflect
            interaction.inputDir = reflect(interaction.outputDir, interaction.normal);
            interaction.sampledLobe = BSDFLobe::Diffuse;
            interaction.iorO = interaction.iorI;
        }
        else
        {
            // Refract
            interaction.inputDir = refract(interaction.outputDir, interaction.normal, VdotN, eta, sinSquaredThetaT);
            interaction.sampledLobe = BSDFLobe::SpecularTransmission;
            interaction.iorO = iorO;
        }

        // TODO: Should check for NaN
    }

    float pdf(SurfaceInteraction const&) const override
    {
        return 1.0f;
    }

    BSDFLobe getSupportedLobes(Vec2f const&) const override
    {
        return BSDFLobe::Specular;
    }
};

class DiffuseDielectric : public Material
{
protected:
    DiffuseDielectric() = default;

public:
    std::shared_ptr<Texture> texture;
    float ior = 1.0f;

    DiffuseDielectric(std::shared_ptr<Texture> const& texture, float ior):
        texture(texture), ior(ior) {};

    Vec3f evaluate(SurfaceInteraction const& interaction) const override
    {
        return texture->evaluate(interaction.uv) * dot(interaction.normal, interaction.inputDir) * (float)M_1_PI;
    }

    void sample(SurfaceInteraction &interaction, Sampler &sampler) const override
    {
        float VdotN = dot(interaction.outputDir, interaction.normal);
        float iorO = ior;

        // Check for side
        if (VdotN < 0.0f)
        {
            iorO = 1.0f;
            interaction.normal = -interaction.normal;
            VdotN = -VdotN;
        }

        float eta = interaction.iorI / iorO;
        float sinSquaredThetaT = SinSquaredThetaT(VdotN, eta);
        float fresnel = Fresnel(interaction.iorI, iorO, VdotN, sinSquaredThetaT);

        float rand = sampler.nextFloat();
        if (rand <= fresnel)
        {
            // Reflect
            //interaction.inputDir = reflect(interaction.outputDir, interaction.normal);
            interaction.inputDir = SampleHemisphere(interaction.normal, sampler);
            interaction.sampledLobe = BSDFLobe::SpecularReflection;
            interaction.iorO = interaction.iorI;
        }
        else
        {
            // Refract
            interaction.inputDir = refract(SampleHemisphere(interaction.normal, sampler), interaction.normal, VdotN, eta, sinSquaredThetaT);
            interaction.sampledLobe = BSDFLobe::SpecularTransmission;
            interaction.iorO = iorO;
        }

        // TODO: Should check for NaN
    }

    float pdf(SurfaceInteraction const& interaction) const override
    {
        return dot(interaction.normal, interaction.inputDir) * (float)M_1_PI;
    }

    BSDFLobe getSupportedLobes(Vec2f const&) const override
    {
        return BSDFLobe::Specular;
    }
};

}

#endif //RAYTRACER_MATERIAL_H

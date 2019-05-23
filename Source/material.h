#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "enums.h"
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
    virtual Vec3f evaluate(ScatterEvent const& interaction) const = 0;
    virtual void sample(ScatterEvent &interaction, Sampler &sampler) const = 0;
    virtual float pdf(ScatterEvent const& interaction) const = 0;

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

    Vec3f evaluate(ScatterEvent const& interaction) const override
    {
        return texture->evaluate(interaction.uv) * std::abs(dot(interaction.normal, interaction.inputDir)) * (float)M_1_PI;
    }

    void sample(ScatterEvent &interaction, Sampler &sampler) const override
    {
        interaction.inputDir = sampleHemisphere(interaction.normal, sampler);
        interaction.sampledLobe = BSDFLobe::Diffuse;
    }

    float pdf(ScatterEvent const& interaction) const override
    {
        return std::abs(dot(interaction.normal, interaction.inputDir)) * (float)M_1_PI;
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

    Vec3f evaluate(ScatterEvent const& interaction) const override
    {
        return texture->evaluate(interaction.uv);
    }

    void sample(ScatterEvent &interaction, Sampler &) const override
    {
        interaction.inputDir = reflect(interaction.outputDir, interaction.normal);
        interaction.sampledLobe = BSDFLobe::SpecularReflection;
    }

    float pdf(ScatterEvent const&) const override
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

    Vec3f evaluate(ScatterEvent const& interaction) const override
    {
        return texture->evaluate(interaction.uv);
    }

    void sample(ScatterEvent &interaction, Sampler &sampler) const override
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

    float pdf(ScatterEvent const&) const override
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

    Vec3f evaluate(ScatterEvent const& interaction) const override
    {
        return texture->evaluate(interaction.uv) * dot(interaction.normal, interaction.inputDir) * (float)M_1_PI;
    }

    void sample(ScatterEvent &interaction, Sampler &sampler) const override
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
            interaction.inputDir = sampleHemisphere(interaction.normal, sampler);
            interaction.sampledLobe = BSDFLobe::SpecularReflection;
            interaction.iorO = interaction.iorI;
        }
        else
        {
            // Refract
            interaction.inputDir = refract(sampleHemisphere(interaction.normal, sampler), interaction.normal, VdotN, eta, sinSquaredThetaT);
            interaction.sampledLobe = BSDFLobe::SpecularTransmission;
            interaction.iorO = iorO;
        }

        // TODO: Should check for NaN
    }

    float pdf(ScatterEvent const& interaction) const override
    {
        return dot(interaction.normal, interaction.inputDir) * (float)M_1_PI;
    }

    BSDFLobe getSupportedLobes(Vec2f const&) const override
    {
        return BSDFLobe::Specular;
    }
};

class Isotropic : public Material
{
protected:
    Isotropic() = default;

public:
    std::shared_ptr<Texture> texture;

    Isotropic(std::shared_ptr<Texture> const& texture):
        texture(texture) {};

    Vec3f evaluate(ScatterEvent const& interaction) const override
    {
        return texture->evaluate(interaction.uv) * (float)M_1_PI;
    }

    void sample(ScatterEvent &interaction, Sampler &sampler) const override
    {
        interaction.inputDir = sampleSphere(sampler);
        interaction.sampledLobe = BSDFLobe::Diffuse;
    }

    float pdf(ScatterEvent const&) const override
    {
        return 4.0f * (float)M_1_PI;
    }

    BSDFLobe getSupportedLobes(Vec2f const&) const override
    {
        return BSDFLobe::Diffuse;
    }
};

class Glossy : public Material
{
public:
    std::shared_ptr<Texture> texture;
    float power;

    Glossy(std::shared_ptr<Texture> const& texture, float power):
        texture(texture), power(power) {};

    Vec3f evaluate(ScatterEvent const& interaction) const override
    {
        //Vec3f reflected = glm::normalize(glm::reflect(ray.dir, normal));
        //float specularLow  = std::pow(glm::dot(-ray.dir, reflected), 10);
        //float specularHigh = std::pow(glm::dot(-ray.dir, reflected), 500);
        Vec3f reflected = normalise(reflect(interaction.outputDir, interaction.normal));
        float specular = std::pow(std::max(0.0f, dot(reflected, interaction.inputDir)), power);
        return texture->evaluate(interaction.uv) * specular * (float)M_1_PI;
    }

    void sample(ScatterEvent &interaction, Sampler &sampler) const override
    {
        interaction.inputDir = sampleHemisphere(interaction.normal, sampler);
        interaction.sampledLobe = BSDFLobe::Diffuse;
    }

    float pdf(ScatterEvent const& interaction) const override
    {
        Vec3f reflected = normalise(reflect(interaction.outputDir, interaction.normal));
        float specular = std::pow(std::max(0.0f, dot(reflected, interaction.inputDir)), power);
        return specular * (float)M_1_PI;
    }

    BSDFLobe getSupportedLobes(Vec2f const&) const override
    {
        return BSDFLobe::Diffuse;
    }
};

class Phong : public Material
{
public:
    std::shared_ptr<Lambert> lambert;
    std::shared_ptr<Glossy> glossy;
    float kd;
    float ks;

    Phong(std::shared_ptr<Lambert> const& lambert, std::shared_ptr<Glossy> glossy, float kd, float ks):
        lambert(lambert), glossy(glossy), kd(kd), ks(ks) {};

    Vec3f evaluate(ScatterEvent const& interaction) const override
    {
        return lambert->evaluate(interaction) * kd + glossy->evaluate(interaction) * ks;
    }

    void sample(ScatterEvent &interaction, Sampler &sampler) const override
    {
        interaction.inputDir = sampleHemisphere(interaction.normal, sampler);
        interaction.sampledLobe = BSDFLobe::Diffuse;
    }

    float pdf(ScatterEvent const& interaction) const override
    {
        return lambert->pdf(interaction) * kd + glossy->pdf(interaction) * ks;
    }

    BSDFLobe getSupportedLobes(Vec2f const&) const override
    {
        return BSDFLobe::Diffuse;
    }
};

/*
class Blinn
{
public:
    Blinn(const float& Exponent) :
        m_Exponent(Exponent) {};

    void SampleF(const Vec3f& Wo, Vec3f& Wi, float& Pdf, Sampler &sampler) const
    {
        // Compute sampled half-angle vector $\wh$ for Blinn distribution
        float costheta = powf(U.x, 1.f / (m_Exponent+1));
        float sintheta = sqrtf(std::max(0.f, 1.f - costheta*costheta));
        float phi = U.y * 2.f * M_PI;

        Vec3f wh = SphericalDirection(sintheta, costheta, phi);

        if (!SameHemisphere(Wo, wh))
            wh = -wh;

        // Compute incident direction by reflecting about $\wh$
        Wi = -Wo + wh * 2.0f * dot(Wo, wh);

        // Compute PDF for $\wi$ from Blinn distribution
        float blinn_pdf = ((m_Exponent + 1.f) * powf(costheta, m_Exponent)) / (2.f * M_PI * 4.f * dot(Wo, wh));

        if (dot(Wo, wh) <= 0.f)
            blinn_pdf = 0.f;

        Pdf = blinn_pdf;
    }

    float Pdf(const Vec3f& Wo, const Vec3f& Wi) const
    {
        Vec3f wh = normalise(Wo + Wi);

        float costheta = AbsCosTheta(wh);
        // Compute PDF for $\wi$ from Blinn distribution
        float blinn_pdf = ((m_Exponent + 1.f) * powf(costheta, m_Exponent)) / (2.f * M_PI * 4.f * dot(Wo, wh));

        if (dot(Wo, wh) <= 0.0f)
            blinn_pdf = 0.0f;

        return blinn_pdf;
    }

    float D(const Vec3f& wh) const
    {
        float costhetah = AbsCosTheta(wh);
        return (m_Exponent+2) * 0.5f * (float)M_1_PI * powf(costhetah, m_Exponent);
    }

    float	m_Exponent;
};

class Microfacet : Material
{
public:
    std::shared_ptr<Texture> texture;
    Blinn m_Blinn;

    Microfacet(std::shared_ptr<Texture> const& texture, const float& Ior, const float& Exponent) :
        texture(texture), m_Blinn(Exponent) {};

    Vec3f evaluate(ScatterEvent const& interaction) const override
    {
        float cosThetaO = AbsCosTheta(interaction.outputDir);
        float cosThetaI = AbsCosTheta(interaction.inputDir);

        if (cosThetaI == 0.f || cosThetaO == 0.f)
            return Vec3f(0.0f, 0.0f, 0.0f);

        Vec3f wh = interaction.inputDir + interaction.outputDir;

        if (wh.x == 0. && wh.y == 0. && wh.z == 0.)
            return Vec3f(0.0f, 0.0f, 0.0f);

        wh = normalise(wh);
        float cosThetaH = dot(interaction.inputDir, wh);

        Vec3f F(1.0f, 1.0f, 1.0f);

        return texture->evaluate(interaction.uv) * m_Blinn.D(wh) * G(interaction.outputDir, interaction.inputDir, wh) * F / (4.f * cosThetaI * cosThetaO);
    }

    void sample(ScatterEvent &interaction, Sampler &sampler) const override
    {
        float Pdf;
        m_Blinn.SampleF(interaction.outputDir, interaction.inputDir, Pdf, sampler);

        if (!SameHemisphere(interaction.outputDir, interaction.inputDir))
            return SPEC_BLACK;

        return this->F(wo, wi);
    }

    float pdf(ScatterEvent const& interaction) const override
    {
        if (!SameHemisphere(interaction.outputDir, interaction.inputDir))
            return 0.0f;

        return m_Blinn.Pdf(interaction.outputDir, interaction.inputDir);
    }

    float G(const Vec3f& wo, const Vec3f& wi, const Vec3f& wh) const
    {
        float NdotWh = AbsCosTheta(wh);
        float NdotWo = AbsCosTheta(wo);
        float NdotWi = AbsCosTheta(wi);
        float WOdotWh = AbsDot(wo, wh);

        return std::min(1.f, std::min((2.f * NdotWh * NdotWo / WOdotWh), (2.f * NdotWh * NdotWi / WOdotWh)));
    }

};*/

}

#endif //RAYTRACER_MATERIAL_H

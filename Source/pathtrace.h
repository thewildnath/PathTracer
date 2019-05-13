#ifndef RAYTRACER_PATHTRACE_H
#define RAYTRACER_PATHTRACE_H

#include "ray.h"
#include "raytrace.h"
#include "sampler.h"
#include "scene.h"
#include "scatterevent.h"
#include "settings.h"
#include "vector_type.h"

namespace scg
{

Vec3f SampleLights(ScatterEvent &interaction, Scene const& scene, std::shared_ptr<Material> const& material, std::shared_ptr<Light> const& hitLight, Settings const& settings, Sampler &sampler)
{
    // Cannot light mirror
    if ((material->getSupportedLobes(interaction.uv) & BSDFLobe::Specular) != 0)
    {
        return Vec3f(0, 0, 0);
    }

    // Cannot sample the hitLight
    if (scene.lights.size() <= 1 && hitLight != nullptr)
    {
        return Vec3f(0, 0, 0);
    }

    // Find another light
    std::shared_ptr<Light> light;
    do
    {
        size_t index = (size_t)sampler.nextDiscrete(scene.lights.size());
        light = scene.lights[index];
    } while (light == hitLight);

    // Calculate light
    Vec3f directLight;

    LightType lightType = light->getType();
    LightHit lightHit = light->illuminate(interaction, sampler);

    switch (lightType)
    {
        case LightType_Abstract:
        {
            if (lightHit.pdf != 0)
            {
                interaction.inputDir = lightHit.direction;
                float pdf = material->pdf(interaction);
                if (pdf != 0)
                {
                    directLight += material->evaluate(interaction) * lightHit.colour / lightHit.pdf; // TODO: fix dotProduct omission
                }
            }

            break;
        }
        case LightType_Point:
        case LightType_Directional:
        case LightType_Object:
        {
            Ray lightRay{interaction.position, lightHit.direction, RAY_EPS};
            Intersection lightIntersection{};

            // Check for objects blocking the path
            if (!getClosestIntersection(scene, lightRay, lightIntersection, settings, sampler, scene.lightIngoreMask) ||
                lightIntersection.distance + EPS >= lightHit.distance)
            {
                if (std::isnormal(lightHit.pdf)) // Real number, not 0
                {
                    interaction.inputDir = lightHit.direction;
                    float pdf = material->pdf(interaction);
                    if (pdf != 0)
                    {
                        directLight += material->evaluate(interaction) * lightHit.colour / lightHit.pdf;
                    }
                }
            }

            break;
        }
    }

    return directLight;
}

Vec3f trace(
    Scene const& scene,
    Ray ray,
    int depth,
    Settings const& settings,
    Sampler &sampler)
{
    Vec3f colour;
    Vec3f throughput(1.0f, 1.0f, 1.0f);

    ScatterEvent interaction;
    interaction.iorI = 1.0f; // Air

    int minBounces = depth;
    int maxBounces = 50;

    for (int bounces = 0; bounces < maxBounces; ++bounces)
    {
        // Intersect the scene
        Intersection intersection;

        if (!getClosestIntersection(scene, ray, intersection, settings, sampler, scene.lightIngoreMask))
        {
            colour += throughput * Vec3f(0, 0, 0); // Ambient //TODO: skybox
            break;
        }

        if (intersection.surfaceType == SurfaceType::Volume)
        {
            Vec3f localPos = intersection.position - scene.volumePos;
            float intensity = scene.volume->sampleVolume(localPos);
            Vec4f out = settings.transferFunction.evaluate(intensity);

            float light = 0.1f; //std::max(0.1f, dot(intersection.normal, settings.lightDir));

            Ray lightRay(intersection.position, -settings.lightDir);

            if (!getClosestIntersection(scene, lightRay, intersection, settings, sampler, scene.lightIngoreMask))
            {
                Vec3f normal = normalise(scene.volume->getGradient(localPos, 0.5f));
                light = std::max(light, dot(normal, settings.lightDir));
            }

            colour += (Vec3f(out.x, out.y,  out.z) * light * 1.0f);

            return colour;
            break;
        }

        auto const& material = scene.materials[intersection.materialID];

        // Initialise interaction
        interaction.position = intersection.position;
        interaction.normal = intersection.normal;
        interaction.uv = interaction.uv;
        interaction.outputDir = -ray.direction;
        interaction.iorO = 0.0f;

        // Add light
        auto const& hitLight = material->getLight(interaction.uv);
        if (hitLight != nullptr)
        {
            colour += throughput * hitLight->getEmittance(interaction);
        }

        // Calculate direct light
        colour += throughput * SampleLights(interaction, scene, material, hitLight, settings, sampler);

        // Sample next direction
        material->sample(interaction, sampler);
        float pdf = material->pdf(interaction);

        // Accumulate
        if (pdf == 0) std::cout << "WTF";
        throughput *= material->evaluate(interaction) / pdf;

        if (interaction.sampledLobe == BSDFLobe::SpecularTransmission)
        {
            interaction.iorI = interaction.iorO;
        }

        // Create new ray
        ray.origin = interaction.position;
        ray.direction = interaction.inputDir;
        ray.minT = RAY_EPS;

        // Russian Roulette
        if (bounces > minBounces)
        {
            float p = std::max(throughput.x, std::max(throughput.y, throughput.z));
            if (sampler.nextFloat() > p) {
                break;
            }

            throughput *= 1 / p;
        }
    }

    return colour / minBounces;
}

}

#endif //RAYTRACER_PATHTRACE_H

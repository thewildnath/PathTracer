#include "raytrace.h"

#include "sampler.h"
#include "surfaceinteraction.h"
#include "vector_type.h"
#include "ray.h"

#include <cassert>
#include <limits>

namespace scg
{

bool getClosestIntersection(
    Scene const& scene,
    Ray const& ray,
    Intersection &closestIntersection)
{
    float minDistance = std::numeric_limits<float>::max();
    int index = -1;

    for (int i = 0; i < (int)scene.objects.size(); ++i)
    {
        Intersection intersection;

        if (scene.objects[i]->getIntersection(ray, intersection))
        {
            if (intersection.distance < minDistance)
            {
                minDistance = intersection.distance;
                index = i;
                closestIntersection = intersection;
            }
        }
    }

    if (index == -1)
    {
        return false;
    }

    closestIntersection.objectID = index;

    return true;
}

inline float powerHeuristic(int numf, float fPdf, int numg, float gPdf) {
    float f = numf * fPdf;
    float g = numg * gPdf;

    return (f * f) / (f * f + g * g);
}

Vec3f SampleLights(SurfaceInteraction &interaction, Scene const& scene, std::shared_ptr<Material> const& material, std::shared_ptr<Light> const& hitLight, Sampler &sampler)
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
            interaction.inputDir = lightHit.direction;
            float pdf = material->pdf(interaction);
            if (lightHit.pdf != 0)
            {
                interaction.inputDir = lightHit.direction;
                float pdf = material->pdf(interaction);
                if (pdf != 0)
                {
                    directLight += material->evaluate(interaction) * lightHit.colour / lightHit.pdf;
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
            if (!scg::getClosestIntersection(scene, lightRay, lightIntersection) ||
                lightIntersection.distance + EPS >= lightHit.distance)
            {
                if (std::isnormal(lightHit.pdf)) // Real number, not 0
                {
                    interaction.inputDir = lightHit.direction;
                    float pdf = material->pdf(interaction);
                    if (pdf != 0)
                    {
                        float weight = M_PI;//powerHeuristic(1, lightHit.pdf, 1, pdf);
                        directLight += material->evaluate(interaction) * lightHit.colour * weight / lightHit.pdf;
                    }
                }
            }

            break;
        }
    }

    return directLight * scene.lights.size();
}

Vec3f trace(
    Scene const& scene,
    Ray ray,
    int depth,
    Sampler &sampler)
{
    Vec3f colour;
    Vec3f throughput(1.0f, 1.0f, 1.0f);

    for (int bounces = 0; bounces < depth; ++bounces)
    {
        // Intersect the scene
        Intersection intersection;

        if (!getClosestIntersection(scene, ray, intersection))
        {
            colour += throughput * Vec3f(0, 0, 0); // Ambient //TODO: skybox
            break;
        }

        auto const& material = scene.materials[intersection.materialID];

        // Initialise interaction
        SurfaceInteraction interaction;
        interaction.position = intersection.position;
        interaction.normal = intersection.normal;
        interaction.uv = interaction.uv;
        interaction.outputDir = -ray.direction;

        // Add light
        auto const& hitLight = material->getLight(interaction.uv);
        if (hitLight != nullptr)
        {
            colour += throughput * hitLight->getEmittance(interaction);
        }

        // TODO: maybe it's wrong???????
        // Reflect normal
        // Done after sampling the light because it only emits on one face
        if (dot(interaction.outputDir, interaction.normal) < 0)
            interaction.normal *= -1;

        // Calculate direct light
        colour += throughput * SampleLights(interaction, scene, material, hitLight, sampler);

        // Sample next direction
        material->sample(interaction, sampler);
        float pdf = material->pdf(interaction);

        // Accumulate
        if (pdf == 0) std::cout << "WTF";
        throughput *= material->evaluate(interaction) / pdf;
        //throughput *= pdf;

        // Create new ray
        ray.origin = interaction.position;
        ray.direction = interaction.inputDir;
        ray.minT = RAY_EPS;
    }

    return colour / depth;
}

}
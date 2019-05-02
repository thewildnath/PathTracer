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

Vec3f SampleLights(SurfaceInteraction &interaction, Scene const& scene, std::shared_ptr<Material> const& material, std::shared_ptr<Light> const& hitLight, Sampler &sampler)
{
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
            //float intensity = lightHit.intensity * std::max(0.0f, dot(interaction.normal, lightHit.direction));
            //directLight += lightHit.colour * intensity;
            interaction.inputDir = lightHit.direction;
            float pdf = material->pdf(interaction);
            if (pdf != 0)
            {
                directLight += lightHit.colour * material->evaluate(interaction) * lightHit.intensity / pdf;
            }

            break;
        }
        case LightType_Point:
        case LightType_Directional:
        case LightType_Object:
        {
            Ray lightRay{interaction.getSafePosition(), lightHit.direction};
            Intersection lightIntersection{};

            // Check for objects blocking the path
            if (!scg::getClosestIntersection(scene, lightRay, lightIntersection) ||
                lightIntersection.distance + EPS >= lightHit.distance)
            {
                //float intensity = lightHit.intensity * std::max(0.0f, dot(interaction.normal, lightHit.direction));
                //directLight += lightHit.colour * intensity;
                interaction.inputDir = lightHit.direction;
                float pdf = material->pdf(interaction);
                if (pdf != 0)
                {
                    directLight += lightHit.colour * material->evaluate(interaction) * lightHit.intensity / pdf;
                }
            }

            break;
        }
    }

    return directLight;
}

Vec3f trace(
    Scene const& scene,
    Ray const& ray,
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

        Vec3f c = material->evaluate(interaction);
        float pdf = material->pdf(interaction);

        // Calculate indirect light
        /*
        Vec3f indirectLight;

        Vec3f Nt, Nb;
        createCoordinateSystem(normal, Nt, Nb);

        float pdf = 1 / (2 * (float)M_PI);
        float r1 = sampler.nextFloat();
        float r2 = sampler.nextFloat();
        Vec3f sample = uniformSampleHemisphere(r1, r2);
        Vec3f nextDirection(
            sample.x * Nb.x + sample.y * normal.x + sample.z * Nt.x,
            sample.x * Nb.y + sample.y * normal.y + sample.z * Nt.y,
            sample.x * Nb.z + sample.y * normal.z + sample.z * Nt.z);
        Ray nextRay{interaction.getSafePosition(), nextDirection};
        // don't forget to divide by PDF and multiply by cos(theta)
        indirectLight = trace(scene, nextRay, depth - 1, sampler) * r1 / pdf;
        //indirectLight /= 255;

        // Finalise and return
        //return (directLight / M_PI + indirectLight * 2) * colour;
        float coef = 0.8;
        return colour * (directLight * coef + indirectLight * (1 - coef));//*/

        break;
    }

    return colour;
}

}
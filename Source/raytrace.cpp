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

void createCoordinateSystem(Vec3f const& N, Vec3f &Nt, Vec3f &Nb)
{
    if (std::fabs(N.x) > std::fabs(N.y))
        Nt = Vec3f(N.z, 0, -N.x) / std::sqrt(N.x * N.x + N.z * N.z);
    else
        Nt = Vec3f(0, -N.z, N.y) / std::sqrt(N.y * N.y + N.z * N.z);
    Nb = cross(N, Nt);
}

Vec3f uniformSampleHemisphere(const float &r1, const float &r2)
{
    // cos(theta) = u1 = y
    // cos^2(theta) + sin^2(theta) = 1 -> sin(theta) = srtf(1 - cos^2(theta))
    float sinTheta = sqrtf(1 - r1 * r1);
    float phi = 2 * (float)M_PI * r2;
    float x = sinTheta * cosf(phi);
    float z = sinTheta * sinf(phi);
    return Vec3f(x, r1, z);
}

Vec3f trace(
    Scene const& scene,
    Ray const& ray,
    int depth,
    Sampler &sampler)
{
    // Check for recursion end
    if (depth <= 0)
    {
        return Vec3f(0, 0, 0); // Ambient
    }

    // Intersect the scene
    Intersection intersection{};

    if (!getClosestIntersection(scene, ray, intersection))
    {
        return Vec3f(0, 0, 0); // Ambient //TODO: skybox
    }

    Vec3f const& normal = intersection.normal;
    auto const& material = scene.materials[intersection.materialID];
    auto const& lightPtr = material->lightPtr;
    Vec3f colour = material->getColour(intersection.uv);

    Vec3f directLight;

    if (lightPtr != nullptr)
    {
        // Do not light on the back side
        if (dot(normal, -ray.direction) >= 0)
            directLight = lightPtr->getEmittance();
    }

    // Calculate interaction(safe point for light calculation and next ray trace)
    SurfaceInteraction interaction{
        intersection.position,
        normal};

    //if (diffuse)
    {
        // Calculate direct light
        size_t index = (size_t)sampler.nextDiscrete(scene.lights.size());
        assert(index < scene.lights.size());

        std::shared_ptr<Light> light = scene.lights[index];
        LightType lightType = light->getType();
        LightHit lightHit = light->illuminate(interaction, sampler);

        if (light != lightPtr)
        switch (lightType)
        {
            case LightType_Abstract:
            {
                float intensity = lightHit.intensity * std::max(0.0f, dot(normal, lightHit.direction));
                directLight += lightHit.colour * intensity;

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
                    float intensity = lightHit.intensity * std::max(0.0f, dot(normal, lightHit.direction));
                    directLight += lightHit.colour * intensity;
                }

                break;
            }
        }

        // Calculate indirect light
        //*
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
        return colour * directLight;
    }
}

}
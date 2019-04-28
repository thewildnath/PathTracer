#include "raytrace.h"

#include "intersection.h"
#include "vector_type.h"
#include "ray.h"

#include <limits>
#include <random>

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

        if (scene.objects[i].getIntersection(ray, intersection))
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


Vec3f trace(
    Scene const& scene,
    Ray const& ray,
    int depth,
    std::default_random_engine &generator,
    std::uniform_real_distribution<float> &distribution)
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
    Material const& material = scene.materials[intersection.materialID];
    Vec3f colour = material.getColour(intersection.uv);

    // Calculate interaction(safe point for light calculation and next ray trace)
    Interaction interaction{
        intersection.position + normal * 0.01f,
        normal};

    //if (diffuse)
    {
        // Calculate direct light
        float directLight = 0;

        float num = distribution(generator) * scene.lights.size();
        int index = (int)std::floor(num);
        if (index < scene.lights.size())
        {
            std::shared_ptr<Light> light = scene.lights[index];
            LightType lightType = light->getType();
            LightHit lightHit = light->illuminate(interaction);

            switch (lightType)
            {
                case LightType_Abstract:
                {
                    float directLightPartial = lightHit.intensity * std::max(0.0f, dot(normal, lightHit.direction));
                    directLight += directLightPartial;

                    break;
                }
                case LightType_Point:
                case LightType_Directional:
                case LightType_Surface:
                {
                    Ray lightRay{interaction.position, lightHit.direction};
                    Intersection lightIntersection{};

                    // Check for objects blocking the path
                    if (!scg::getClosestIntersection(scene, lightRay, lightIntersection) ||
                        lightIntersection.distance >= lightHit.distance + EPS)
                    {
                        float directLightPartial =
                            lightHit.intensity * std::max(0.0f, dot(normal, lightHit.direction));
                        directLight += directLightPartial;
                    }

                    break;
                }
            }
        }
        // Calculate indirect light

        // Finalise and return
        return colour * directLight;
    }
}

}
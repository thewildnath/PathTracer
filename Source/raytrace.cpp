#include "raytrace.h"

#include "ray.h"
#include "raycast.h"
#include "sampler.h"
#include "settings.h"
#include "scatterevent.h"
#include "vector_type.h"

#include <cassert>
#include <limits>

namespace scg
{

bool getClosestIntersection(
    Scene const& scene,
    Ray const& ray,
    Intersection &closestIntersection,
    Settings const& settings,
    Sampler &sampler)
{
    float minDistance = std::numeric_limits<float>::max();
    int index = -1;

    Intersection intersection;
//*
    if (scene.volume)
    {
        Ray volumeRay{ray.origin - scene.volumePos, ray.direction};
        if ((settings.renderType == 1 && castRayWoodcockFast(*scene.volume, volumeRay, intersection, settings, sampler)) ||
            (settings.renderType == 2 && castRayWoodcockFast2(*scene.volume, volumeRay, intersection, settings, sampler)))
        {
            minDistance = intersection.distance;
            index = (int) scene.objects.size();
            closestIntersection = intersection;
            closestIntersection.position += scene.volumePos;
        }
    }
//*/
    for (int i = 0; i < (int)scene.objects.size(); ++i)
    {
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

}
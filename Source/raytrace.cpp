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
    Sampler &sampler,
    int ignore = 0)
{
    float minDistance = std::numeric_limits<float>::max();
    int index = -1;

    Intersection intersection;
/*
    if (castRayWoodcockFast(*scene.volume, ray, intersection, settings, sampler))
    {
        minDistance = intersection.distance;
        index = scene.objects.size();
        closestIntersection = intersection;
    }
*/
    for (int i = 0; i < (int)scene.objects.size(); ++i)
    {
        if (scene.objects[i]->getIntersection(ray, intersection, ignore))
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
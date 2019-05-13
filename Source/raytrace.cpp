#include "raytrace.h"

#include "ray.h"
#include "raytrace.h"
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
    int ignore = 0)
{
    float minDistance = std::numeric_limits<float>::max();
    int index = -1;

    for (int i = 0; i < (int)scene.objects.size(); ++i)
    {
        Intersection intersection;

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
#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "intersection.h"
#include "scene.h"
#include "vector_type.h"
#include "ray.h"

#include <vector>

namespace scg
{

bool getClosestIntersection(
    Scene const& scene,
    Ray const& ray,
    Intersection& closestIntersection,
    int ignore);

}

#endif //RAYTRACE_H

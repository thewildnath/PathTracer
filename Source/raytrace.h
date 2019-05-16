#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "intersection.h"
#include "sampler.h"
#include "scene.h"
#include "settings.h"
#include "vector_type.h"
#include "ray.h"

#include <vector>

namespace scg
{

bool getClosestIntersection(
    Scene const& scene,
    Ray const& ray,
    Intersection& closestIntersection,
    Settings const& settings,
    Sampler &sampler);

}

#endif //RAYTRACE_H

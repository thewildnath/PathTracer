#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "intersection.h"
#include "object.h"
#include "sampler.h"
#include "scene.h"
#include "triangle.h"
#include "vector_type.h"
#include "ray.h"

#include <vector>

namespace scg
{

bool getClosestIntersection(
    Scene const& scene,
    Ray const& ray,
    Intersection &closestIntersection);

Vec3f trace(
    Scene const& scene,
    Ray const& ray,
    int depth,
    Sampler &sampler);
}

#endif //RAYTRACE_H

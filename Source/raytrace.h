#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "intersection.h"
#include "object.h"
#include "scene.h"
#include "triangle.h"
#include "vector_type.h"
#include "ray.h"

#include <random>
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
    std::default_random_engine &generator,
    std::uniform_real_distribution<float> &distribution);
}

#endif //RAYTRACE_H

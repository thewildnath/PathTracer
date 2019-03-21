#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "intersection.h"
#include "object.h"
#include "triangle.h"
#include "vector_type.h"
#include "ray.h"

#include <vector>

namespace scg
{

bool getClosestIntersection(
    Ray const& ray,
    std::vector<Object> const& objects,
    Intersection &closestIntersection,
    Material& material);

Vec3f trace(
    Ray const& ray,
    std::vector<Object> const& objects,
    int depth);
}

#endif //RAYTRACE_H

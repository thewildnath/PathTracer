#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "triangle.h"
#include "utils.h"
#include "vector_type.h"
#include "ray.h"

#include <vector>

namespace scg
{

bool getClosestIntersection(
    Ray const& ray,
    std::vector <Triangle> const& triangles,
    Intersection &closestIntersection);

}

#endif //RAYTRACE_H

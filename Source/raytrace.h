#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "triangle.h"
#include "utils.h"

#include <glm/glm.hpp>

#include <vector>

namespace scg
{

bool getClosestIntersection(
    glm::vec4 start,
    glm::vec4 dir,
    const std::vector <Triangle> &triangles,
    Intersection &closestIntersection);

}

#endif //RAYTRACE_H

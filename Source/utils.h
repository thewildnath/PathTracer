#ifndef COMS30115_UTILITY_H
#define COMS30115_UTILITY_H

#include "triangle.h"

#include <glm/glm.hpp>

#include <vector>

namespace scg
{

struct Intersection
{
    glm::vec4 position;
    float distance;
    int triangleIndex;
};

// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
void LoadTestModel(std::vector<Triangle> &triangles);

}

#endif //COMS30115_UTILITY_H

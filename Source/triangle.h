#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include <glm/glm.hpp>

namespace scg
{

// Used to describe a triangular surface:
class Triangle
{
public:
    glm::vec4 v0;
    glm::vec4 v1;
    glm::vec4 v2;
    glm::vec4 normal;
    glm::vec3 color;

    Triangle(glm::vec4 v0, glm::vec4 v1, glm::vec4 v2, glm::vec3 color);

    void ComputeNormal();
};

}

#endif //RAYTRACER_TRIANGLE_H

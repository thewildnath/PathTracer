#include "triangle.h"

namespace scg
{

    Triangle::Triangle(glm::vec4 v0, glm::vec4 v1, glm::vec4 v2, glm::vec3 color)
        : v0(v0), v1(v1), v2(v2), color(color)
    {
        ComputeNormal();
    }

    void Triangle::ComputeNormal()
    {
        glm::vec3 e1 = glm::vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        glm::vec3 e2 = glm::vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        glm::vec3 normal3 = glm::normalize(glm::cross(e2, e1));
        normal.x = normal3.x;
        normal.y = normal3.y;
        normal.z = normal3.z;
        normal.w = 1.0;
    }

}
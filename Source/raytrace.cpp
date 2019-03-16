#include "raytrace.h"

#include <limits>

namespace scg
{

bool getClosestIntersection(
    glm::vec4 start,
    glm::vec4 dir,
    const std::vector <Triangle> &triangles,
    Intersection &closestIntersection)
{
    float minDistance = std::numeric_limits<float>::max();
    int index = -1;

    for (int i = 0; i < triangles.size(); ++i)
    {
        Triangle triangle = triangles[i];

        glm::vec4 v0 = triangle.v0;
        glm::vec4 v1 = triangle.v1;
        glm::vec4 v2 = triangle.v2;

        glm::vec3 d = glm::vec3(dir);
        glm::vec3 e1 = glm::vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        glm::vec3 e2 = glm::vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        glm::vec3 b = glm::vec3(start.x - v0.x, start.y - v0.y, start.z - v0.z);
        glm::mat3 A(-d, e1, e2);
        glm::vec3 x = glm::inverse(A) * b;

        float t = x.x;
        float u = x.y;
        float v = x.z;

        if (0 <= u && 0 <= v && u + v <= 1)
        {
            if (t < minDistance)
            {
                minDistance = t;
                index = i;
            }
        }
    }

    if (index == -1)
    {
        return false;
    }

    closestIntersection.distance = minDistance;
    closestIntersection.position = start + dir * minDistance;
    closestIntersection.triangleIndex = index;

    return true;
}

}
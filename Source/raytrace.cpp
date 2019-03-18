#include "raytrace.h"

#include "intersection.h"
#include "vector_type.h"
#include "ray.h"

#include <limits>

namespace scg
{

bool getClosestIntersection(
    Ray const& ray,
    std::vector<Object> const& objects,
    Intersection &closestIntersection,
    Material& material)
{
    float minDistance = std::numeric_limits<float>::max();
    int index = -1;

    for (int i = 0; i < (int)objects.size(); ++i)
    {
        Intersection intersection;

        if (objects[i].getIntersection(ray, intersection))
        {
            if (intersection.distance < minDistance)
            {
                minDistance = intersection.distance;
                index = i;
                closestIntersection = intersection;
            }
        }
    }

    if (index == -1)
    {
        return false;
    }

    material = objects[index].material;

    return true;
}

/*
bool getClosestIntersection(
    Vec3f start,
    Vec3f dir,
    const std::vector <Triangle> &triangles,
    Intersection &closestIntersection)
{
    float minDistance = std::numeric_limits<float>::max();
    int index = -1;

    for (int i = 0; i < triangles.size(); ++i)
    {
        Triangle triangle = triangles[i];

        Vec3f v0 = triangle.v0;
        Vec3f v1 = triangle.v1;
        Vec3f v2 = triangle.v2;

        glm::vec3 d = glm::vec3(dir.x, dir.y, dir.z);
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
*/
}
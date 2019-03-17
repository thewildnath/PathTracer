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

    //closestIntersection.distance = minDistance;
    //closestIntersection.position = ray(minDistance);
    material = objects[index].material;

    return true;
}


/*
//Möller–Trumbore intersection algorithm
bool getClosestIntersection(
    Ray const& ray,
    std::vector<Object> const& object,
    Intersection &closestIntersection)
{
    float minDistance = std::numeric_limits<float>::max();
    int index = -1;

    for (int i = 0; i < (int)triangles.size(); ++i)
    {
        const float EPSILON = 0.0000001f;
        Vec3f vertex0 = triangles[i].v0;
        Vec3f vertex1 = triangles[i].v1;
        Vec3f vertex2 = triangles[i].v2;
        Vec3f edge1, edge2, h, s, q;
        float a, f, u, v;

        edge1 = vertex1 - vertex0;
        edge2 = vertex2 - vertex0;

        h = cross(ray.direction, edge2);
        a = dot(edge1, h);

        if (a > -EPSILON && a < EPSILON)
            continue;

        f = 1.0f / a;
        s = ray.origin - vertex0;
        u = f * (dot(s, h));

        if (u < 0.0 || u > 1.0)
            continue;

        q = cross(s, edge1);
        v = f * dot(ray.direction, q);

        if (v < 0.0 || u + v > 1.0)
            continue;

        // At this stage we can compute t to find out where the intersection point is on the line.
        float t = f * dot(edge2, q);

        if (t > EPSILON) // Ray intersection
        {
            if (t < minDistance)
            {
                minDistance = t;
                index = i;
            }
        }
        // else: it means that there is a line intersection but not a ray intersection.
    }

    if (index == -1)
    {
        return false;
    }

    closestIntersection.distance = minDistance;
    closestIntersection.position = ray(minDistance);
    closestIntersection.triangleIndex = index;

    return true;
}
*/
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
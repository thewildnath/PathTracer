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
    Intersection &closestIntersection)
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

    closestIntersection.objectID = index;

    return true;
}

// Uniform sampling on a hemisphere to produce outgoing ray directions.
// courtesy of http://www.rorydriscoll.com/2009/01/07/better-sampling/
Vec3f hemisphere(float u1, float u2)
{
    const float r = std::sqrt(1.0f - u1 * u1);
    const float phi = 2.0f * (float)M_PI * u2;
    return Vec3f(std::cos(phi) * r, std::sin(phi) * r, u1);
}

void ons(Vec3f const& v1, Vec3f& v2, Vec3f& v3)
{
    if (std::abs(v1.x) > std::abs(v1.y)) {
        // project to the y = 0 plane and construct a normalized orthogonal vector in this plane
        float invLen = 1.0f / sqrtf(v1.x * v1.x + v1.z * v1.z);
        v2 = Vec3f(-v1.z * invLen, 0.0f, v1.x * invLen);
    } else {
        // project to the x = 0 plane and construct a normalized orthogonal vector in this plane
        float invLen = 1.0f / sqrtf(v1.y * v1.y + v1.z * v1.z);
        v2 = Vec3f(0.0f, v1.z * invLen, -v1.y * invLen);
    }

    v3 = Vec3f(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x);
}

Vec3f trace(
    Ray const& ray,
    std::vector<Object> const& objects,
    int depth)
{
    if (depth <= 0)
    {
        Ray newRay(ray.origin, objects[1].position - ray.origin);

        Intersection intersection;
        Material material;

        if (!getClosestIntersection(newRay, objects, intersection))
        {
            return material.getColour(intersection.uv) * material.emission;
        }

        return Vec3f(0, 0, 0); // Ambient
    }

    Intersection intersection;

    if (!getClosestIntersection(ray, objects, intersection))
    {
        return Vec3f(0, 0, 0); // Ambient
    }

    //Vec3f colour = material.getColour(intersection.uv) * material.emission;
    Vec3f const& normal = intersection.normal;

    // Diffuse
    Vec3f rotX, rotY;
    ons(normal, rotX, rotY);
    Vec3f sampledDir(1, 1, 1);//= hemisphere(RND2,RND2);
    Vec3f rotatedDir;
    rotatedDir.x = dot(Vec3f(rotX.x, rotY.x, normal.x), sampledDir);
    rotatedDir.y = dot(Vec3f(rotX.y, rotY.y, normal.y), sampledDir);
    rotatedDir.z = dot(Vec3f(rotX.z, rotY.z, normal.z), sampledDir);

    rotatedDir = normalise(rotatedDir); // TODO: remove?

    Ray newRay;
    newRay.origin = intersection.position + intersection.normal * 0.001f;
    newRay.direction = normal; //rotatedDir;	// already normalized

    double cost = dot(newRay.direction, normal);

    Vec3f tmp = trace(newRay, objects, depth - 1);

    //colour = colour + multiply(tmp, material.getColour(intersection.uv)) * cost * 0.1f;

    return scg::Vec3f(1, 1, 1);//colour;
}

}
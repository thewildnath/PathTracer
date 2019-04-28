#ifndef RAYTRACER_GEOMETRY_H
#define RAYTRACER_GEOMETRY_H

#include "intersection.h"
#include "ray.h"
#include "triangle.h"

#include <vector>

namespace scg
{

class Geometry
{
public:
    virtual bool getIntersection(Ray const&, Intersection&) const = 0;
};

class Sphere : public Geometry
{
public:
    float radius;

    size_t materialID;

    Sphere(float radius, size_t materialID):
        radius(radius), materialID(materialID) {};

    bool getIntersection(Ray const& ray, Intersection& intersection) const override
    {
        //std::cout << "Circle" << std::endl;
        float b = dot(ray.origin * 2.0f, ray.direction);
        float c = dot(ray.origin, ray.origin) - radius * radius;
        float disc = b * b - 4 * c;

        if (disc < 0)
            return false;

        disc = std::sqrt(disc);
        float sol1 = -b + disc;
        float sol2 = -b - disc;

        float t;
        if (sol2 > EPS)
            t = sol2 / 2.0f;
        else if (sol1 > EPS)
            t = sol1 / 2.0f;
        else
            return false;

        intersection.position   = ray(t);
        intersection.distance   = t;
        intersection.normal     = normalise(intersection.position);
        intersection.materialID = materialID;
        intersection.uv         = Vec2f(0, 0);

        return true;
    }
};

class Mesh : public Geometry
{
public:
    std::vector<Triangle> triangles;

    Mesh() = default;

    Mesh(std::vector<Triangle>& triangles):
        triangles(std::move(triangles)) {};

    bool getIntersection(Ray const& ray, Intersection& intersection) const override
    {
        //std::cout << "Mesh" << std::endl;
        //Möller–Trumbore intersection algorithm
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

        intersection.position   = ray(minDistance);
        intersection.distance   = minDistance;
        intersection.normal     = triangles[index].normal;
        intersection.materialID = triangles[index].materialID;
        intersection.uv         = Vec2f(0, 0);

        return true;
    }
};

}

#endif //RAYTRACER_GEOMETRY_H

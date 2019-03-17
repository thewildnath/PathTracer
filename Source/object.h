#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "intersection.h"
#include "math_utils.h"
#include "ray.h"

namespace scg
{

class Material
{
public:
    Vec3f color;

    Material() = default;

    Material(Vec3f const& color):
        color(color) {};

    Vec3f getColor(Vec2f const& uv) const
    {
        return color;
    }
};

class Geometry
{
public:
    virtual bool getIntersection(Ray const&, Intersection&) const = 0;
};

class Sphere : public Geometry
{
public:
    float radius;

    Sphere(float radius):
        radius(radius) {};

    bool getIntersection(Ray const& ray, Intersection& intersection) const
    {
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

        intersection.position = ray(t);
        intersection.distance = t;
        intersection.normal = normalise(intersection.position);
        intersection.uv.u = 0;
        intersection.uv.v = 0;

        return true;
    }
};

class Object
{
public:
    Vec3f position;

    Material material;
    Geometry const& geometry;

    Object() = default;

    Object(Vec3f position, Material material, Geometry const& geometry):
        position(position), material(material), geometry(geometry) {};

    bool getIntersection(Ray ray, Intersection& intersection) const
    {
        ray.origin -= position;
        return geometry.getIntersection(ray, intersection);
    }
};

}

#endif //RAYTRACER_OBJECT_H

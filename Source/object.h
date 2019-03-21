#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "intersection.h"
#include "geometry.h"
#include "material.h"
#include "math_utils.h"
#include "ray.h"

#include <vector>

namespace scg
{

class Object
{
public:
    Vec3f position;

    Material material;
    Geometry const& geometry;

    Object(Vec3f const& position, Material const& material, Geometry const& geometry):
        position(position), material(material), geometry(geometry) {};

    bool getIntersection(Ray ray, Intersection& intersection) const
    {
        ray.origin -= position;
        return geometry.getIntersection(ray, intersection);
    }
};

}

#endif //RAYTRACER_OBJECT_H

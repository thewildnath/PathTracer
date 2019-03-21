#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "intersection.h"
#include "geometry.h"
#include "material.h"
#include "math_utils.h"
#include "ray.h"

#include <memory>
#include <vector>

namespace scg
{

class Object
{
public:
    Vec3f position;

    std::vector<Material> materials;
    std::shared_ptr<Geometry> geometry;

    Object(
        Vec3f const& position,
        std::vector<Material> materials,
        std::shared_ptr<Geometry> geometry):
        position(position), materials(std::move(materials)), geometry(std::move(geometry)) {};

    bool getIntersection(Ray ray, Intersection& intersection) const
    {
        ray.origin -= position;
        return geometry->getIntersection(ray, intersection);
    }
};

}

#endif //RAYTRACER_OBJECT_H

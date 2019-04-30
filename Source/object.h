#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "intersection.h"
#include "geometry.h"
#include "math_utils.h"
#include "ray.h"

#include <memory>
#include <random>
#include <vector>

namespace scg
{

class Object
{
public:
    Vec3f position;

    std::shared_ptr<Geometry> geometry;

    Object(
        Vec3f const& position,
        std::shared_ptr<Geometry> geometry):
        position(position), geometry(geometry) {};

    bool getIntersection(Ray ray, Intersection& intersection) const
    {
        ray.origin -= position;

        if (!geometry->getIntersection(ray, intersection))
        {
            return false;
        }

        intersection.position += position;

        return true;
    }

    Vec3f sampleSurface(std::default_random_engine &generator, std::uniform_real_distribution<float> &distribution)
    {
        Vec3f point = geometry->sampleSurface(generator, distribution) + position;
        return point;
    }
};

}

#endif //RAYTRACER_OBJECT_H

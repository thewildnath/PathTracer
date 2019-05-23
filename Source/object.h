#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "boundingbox.h"
#include "intersection.h"
#include "geometry.h"
#include "math_utils.h"
#include "ray.h"
#include "sampler.h"
#include "scatterevent.h"

#include <memory>
#include <vector>

namespace scg
{

class Object
{
public:
    Vec3f position;

    std::shared_ptr<Geometry> geometry;

    BoundingBox boundingBox;

    Object(
        Vec3f const& position,
        std::shared_ptr<Geometry> const& geometry):
        position(position), geometry(geometry)
    {
        //boundingBox = geometry->getBoundingBox();
    };

    bool getIntersection(Ray ray, Intersection& intersection) const
    {
        ray.origin -= position;

        //BBIntersection bbIntersection;
        //boundingBox.getIntersection(ray, bbIntersection);

        if (/*!bbIntersection.valid ||*/ !geometry->getIntersection(ray, intersection))
        {
            return false;
        }

        intersection.position += position;

        return true;
    }

    ScatterEvent sampleSurface(Sampler &sampler)
    {
        ScatterEvent interaction = geometry->sampleSurface(sampler);
        interaction.position += position;

        return interaction;
    }
};

}

#endif //RAYTRACER_OBJECT_H

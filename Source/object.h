#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "boundingbox.h"
#include "intersection.h"
#include "geometry.h"
#include "math_utils.h"
#include "ray.h"
#include "sampler.h"
#include "surfaceinteraction.h"

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
        boundingBox = geometry->getBoundingBox();
    };

    bool getIntersection(Ray ray, Intersection& intersection, int ignore = 0) const
    {
        ray.origin -= position;

        if (!boundingBox.getIntersection(ray) || !geometry->getIntersection(ray, intersection, ignore))
        {
            return false;
        }

        intersection.position += position;

        return true;
    }

    SurfaceInteraction sampleSurface(Sampler &sampler)
    {
        SurfaceInteraction interaction = geometry->sampleSurface(sampler);
        interaction.position += position;

        return interaction;
    }
};

}

#endif //RAYTRACER_OBJECT_H

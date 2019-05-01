#ifndef RAYTRACER_INTERACTION_H
#define RAYTRACER_INTERACTION_H

#include "vector_type.h"

namespace scg
{

// Similar to an intersection, but should be initialised with a safe position(i.e. position + normal * EPS)
class SurfaceInteraction
{
public:
    Vec3f position;
    Vec3f normal;

    SurfaceInteraction(Vec3f const& position, Vec3f const& normal):
        position(position), normal(normal) {};

    Vec3f getSafePosition()
    {
        return position + normal * EPS;
    }
};

}

#endif //RAYTRACER_INTERACTION_H

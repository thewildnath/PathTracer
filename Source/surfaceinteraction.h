#ifndef RAYTRACER_INTERACTION_H
#define RAYTRACER_INTERACTION_H

#include "bsdf_lobe.h"
#include "vector_type.h"

namespace scg
{

// Information regarding an interaction with the scene
// May not initialise all fields and update them during the process
class SurfaceInteraction
{
public:
    Vec3f position;
    Vec3f normal;
    Vec2f uv;

    Vec3f inputDir;
    Vec3f outputDir;

    float iorI;
    float iorO;

    BSDFLobe sampledLobe;

    SurfaceInteraction() = default;

    SurfaceInteraction(Vec3f const& position, Vec3f const& normal):
        position(position), normal(normal) {};

    Vec3f getSafePosition() const
    {
        return position + normal * EPS;
    }
};

}

#endif //RAYTRACER_INTERACTION_H

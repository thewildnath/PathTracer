#ifndef RAYTRACER_INTERACTION_H
#define RAYTRACER_INTERACTION_H

#include "enums.h"
#include "vector_type.h"

namespace scg
{

// Information regarding an interaction with the scene
// May not initialise all fields and update them during the process
class ScatterEvent
{
public:
    Vec3f position;
    Vec3f normal;
    Vec2f uv;

    Vec3f inputDir;
    Vec3f outputDir;

    float iorI;
    float iorO;

    SurfaceType surfaceType;
    BSDFLobe sampledLobe;

    ScatterEvent() = default;

    ScatterEvent(Vec3f const& position, Vec3f const& normal, SurfaceType surfaceType):
        position(position), normal(normal), surfaceType(surfaceType) {};

    Vec3f getSafePosition() const
    {
        return position + normal * EPS;
    }
};

}

#endif //RAYTRACER_INTERACTION_H

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "vector_type.h"

namespace scg
{

class Material
{
public:
    Vec3f color;
    double emission;

    Material() = default;

    Material(Vec3f const& color):
        color(color) {};

    Material(Vec3f const& color, float emission):
        color(color), emission(emission) {};

    Vec3f getColour(Vec2f const& uv) const
    {
        return color;
    }
};

}

#endif //RAYTRACER_MATERIAL_H

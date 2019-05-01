#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "light.h"
#include "vector_type.h"

#include <memory>

namespace scg
{

class Material
{
public:
    Vec3f color;
    std::shared_ptr<Light> lightPtr;

    Material() = default;

    Material(Vec3f const& color):
        color(color) {};

    Material(Vec3f const& color, std::shared_ptr<Light> lightPtr):
        color(color), lightPtr(lightPtr) {};

    Vec3f getColour(Vec2f const&) const
    {
        return color;
    }
};

}

#endif //RAYTRACER_MATERIAL_H

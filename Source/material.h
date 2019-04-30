#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "light.h"
#include "vector_type.h"

#include <memory>

namespace scg
{

class Material
{
private:
    Vec3f color;
    std::shared_ptr<Light> lightPtr;

public:
    Material() = default;

    Material(Vec3f const& color):
        color(color) {};

    Material(Vec3f const& color, std::shared_ptr<Light> lightPtr):
        color(color), lightPtr(lightPtr) {};

    Vec3f getColour(Vec2f const& uv) const
    {
        return color;
    }
};

}

#endif //RAYTRACER_MATERIAL_H

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "vector_type.h"

namespace scg
{

class Ray
{
public:
    Vec3f origin;
    Vec3f direction;

    Ray() = default;

    Ray(Vec3f const& origin, Vec3f const& direction):
        origin(origin), direction(normalise(direction)) {};

    inline Vec3f operator()(float const t) const
    {
        return origin + direction * t;
    }
};

}

#endif //RAYTRACER_RAY_H

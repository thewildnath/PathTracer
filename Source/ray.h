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
    float minT;
    float maxT;

    Ray() = default;

    Ray(Vec3f const& origin, Vec3f const& direction, float minT = 0, float maxT = INF):
        origin(origin), direction(normalise(direction)), minT(minT), maxT(maxT) {};

    inline Vec3f operator()(float const t) const
    {
        return origin + direction * t;
    }

    inline bool isInside(float distance) const
    {
        return minT - EPS <= distance && distance <= maxT + EPS;
    }
};

}

#endif //RAYTRACER_RAY_H

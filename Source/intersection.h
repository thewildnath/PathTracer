#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H

#include "vector_type.h"

namespace scg
{

struct Intersection
{
    // Intersection information
    int objectID;
    Vec3f position;
    float distance;

    // Geometry normal at intersection point
    Vec3f normal;

    // Material information
    size_t materialID;
    Vec2f uv;
};

}
#endif //RAYTRACER_INTERSECTION_H

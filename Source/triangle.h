#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "vector_type.h"

namespace scg
{

// Used to describe a triangular surface:
class Triangle
{
public:
    Vec3f v0;
    Vec3f v1;
    Vec3f v2;
    Vec3f normal;
    Vec3f colour;

    Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f colour);

    void ComputeNormal();
};

}

#endif //RAYTRACER_TRIANGLE_H

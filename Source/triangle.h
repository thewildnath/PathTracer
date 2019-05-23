#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "vector_type.h"

namespace scg
{

// Used to describe a triangular surface:
class Triangle
{
public:
    Vec3f v0; Vec2f uv0;
    Vec3f v1; Vec2f uv1;
    Vec3f v2; Vec2f uv2;
    Vec3f normal;

    size_t materialID;

    Triangle(Vec3f v0, Vec3f v1, Vec3f v2, size_t materialID);

    Triangle(Vec3f v0, Vec2f uv0, Vec3f v1, Vec2f uv1, Vec3f v2, Vec2f uv2, size_t materialID);

    void ComputeNormal();
};

}

#endif //RAYTRACER_TRIANGLE_H

#include "triangle.h"

namespace scg
{

    Triangle::Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f color)
        : v0(v0), v1(v1), v2(v2), color(color)
    {
        ComputeNormal();
    }

    void Triangle::ComputeNormal()
    {
        Vec3f e1 = Vec3f(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        Vec3f e2 = Vec3f(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        Vec3f normal3 = cross(e2, e1).normalise();
        normal.x = normal3.x;
        normal.y = normal3.y;
        normal.z = normal3.z;
        normal.w = 1.0;
    }

}
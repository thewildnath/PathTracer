#include "triangle.h"

namespace scg
{

Triangle::Triangle(Vec3f v0, Vec3f v1, Vec3f v2, size_t materialID)
    : v0(v0), v1(v1), v2(v2), materialID(materialID)
{
    ComputeNormal();
}

Triangle::Triangle(Vec3f v0, Vec2f uv0, Vec3f v1, Vec2f uv1, Vec3f v2, Vec2f uv2, size_t materialID)
    : v0(v0), uv0(uv0), v1(v1), uv1(uv1), v2(v2), uv2(uv2), materialID(materialID)
{
    ComputeNormal();
}

void Triangle::ComputeNormal()
{
    Vec3f e1 = Vec3f(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
    Vec3f e2 = Vec3f(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
    Vec3f normal3 = normalise(cross(e2, e1));
    normal.x = normal3.x;
    normal.y = normal3.y;
    normal.z = normal3.z;
    normal.w = 1.0;
}

}
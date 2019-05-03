#ifndef RAYTRACER_BOUNDINGBOX_H
#define RAYTRACER_BOUNDINGBOX_H

#include "ray.h"
#include "vector_type.h"

namespace scg
{

inline Vec3f minV(Vec3f const& v1, Vec3f const& v2)
{
    return Vec3f(
        fminf(v1.x, v2.x),
        fminf(v1.y, v2.y),
        fminf(v1.z, v2.z)
    );
}

inline Vec3f maxV(Vec3f const& v1, Vec3f const& v2)
{
    return Vec3f(
        fmaxf(v1.x, v2.x),
        fmaxf(v1.y, v2.y),
        fmaxf(v1.z, v2.z)
    );
}

class BoundingBox
{
public:
    Vec3f min;
    Vec3f max;
    Vec3f mid;

    BoundingBox() {};

    BoundingBox(Vec3f const& min, Vec3f const& max)
    {
        this->min = min;
        this->max = max;
        this->mid = (min + max) / 2.0f;
    }

    bool getIntersection(Ray const& ray) const
    {
        // Assume NaN can only occur if INF(invR) * 0(dist) => intersect because dist = 0
        Vec3f invR		= Vec3f(1.0f) / ray.direction;
        Vec3f dMin = this->min - ray.origin;
        Vec3f dMax = this->max - ray.origin;

        Vec3f bottomT = invR * dMin;
        bottomT.x = dMin.x == 0 ? 0 : bottomT.x;
        bottomT.y = dMin.y == 0 ? 0 : bottomT.y;
        bottomT.z = dMin.z == 0 ? 0 : bottomT.z;

        Vec3f topT = invR * dMax;
        topT.x = dMax.x == 0 ? 0 : topT.x;
        topT.y = dMax.y == 0 ? 0 : topT.y;
        topT.z = dMax.z == 0 ? 0 : topT.z;

        Vec3f minT		= minV(topT, bottomT);//TopT.Min(BottomT);
        Vec3f maxT		= maxV(topT, bottomT);//TopT.Max(BottomT);
        float largestMinT = fmaxf(fmaxf(minT.x, minT.y), fmaxf(minT.x, minT.z));
        float smallestMaxT = fminf(fminf(maxT.x, maxT.y), fminf(maxT.x, maxT.z));

        if (smallestMaxT < largestMinT)
            return false;

        if (smallestMaxT < ray.minT || largestMinT > ray.maxT)
            return false;

        return true;
    }
};


}

#endif //RAYTRACER_BOUNDINGBOX_H

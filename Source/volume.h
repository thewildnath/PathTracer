#ifndef RAYTRACER_VOLUME_H
#define RAYTRACER_VOLUME_H

#include "octree.h"
#include "settings.h"

#define VOLUME_SIZE 512
#define V_EPS 2

namespace scg
{

class Volume
{
public:
    int height = VOLUME_SIZE;
    int width = VOLUME_SIZE;
    int depth = VOLUME_SIZE;

    float data[VOLUME_SIZE][VOLUME_SIZE][VOLUME_SIZE];

    Octree octree;

    Volume(int height, int width, int depth);

    inline float sampleVolume(Vec3f const &pos) const
    {
        int px = (int)(pos.x - 0.5f);
        int py = (int)(pos.y - 0.5f);
        int pz = (int)(pos.z - 0.5f);

        //return data[px][py][pz];

        float dx = pos.x - px - 0.5f;
        float dy = pos.y - py - 0.5f;
        float dz = pos.z - pz - 0.5f;

        float c000 = data[px][py][pz];
        float c001 = data[px][py][pz + 1];
        float c010 = data[px][py + 1][pz];
        float c011 = data[px][py + 1][pz + 1];
        float c100 = data[px + 1][py][pz];
        float c101 = data[px + 1][py][pz + 1];
        float c110 = data[px + 1][py + 1][pz];
        float c111 = data[px + 1][py + 1][pz + 1];

        float c00 = lerp(c000, c100, dx);
        float c01 = lerp(c001, c101, dx);
        float c10 = lerp(c010, c110, dx);
        float c11 = lerp(c011, c111, dx);

        float c0 = lerp(c00, c10, dy);
        float c1 = lerp(c01, c11, dy);

        float coef =  lerp(c0, c1, dz);

        return coef;
    }

    inline Vec3f getGradient(Vec3f const& pos, float eps) const
    {
        Vec3f deltaX(eps, 0, 0);
        Vec3f deltaY(0, eps, 0);
        Vec3f deltaZ(0, 0, eps);

        return Vec3f(
            sampleVolume(pos - deltaX) - sampleVolume(pos + deltaX),
            sampleVolume(pos - deltaY) - sampleVolume(pos + deltaY),
            sampleVolume(pos - deltaZ) - sampleVolume(pos + deltaZ));
    }

    inline Vec3f getGradientNormalised(Vec3f const& pos, float eps) const
    {
        return normalise(getGradient(pos, eps));
    }
};

void buildOctree(Volume const& volume, Octree &octree, int levels, Settings const& settings);

}

#endif //RAYTRACER_VOLUME_H

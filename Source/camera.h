#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "sampler.h"
#include "ray.h"

namespace scg
{

class Camera
{
public:
    Vec3f position;
    Vec3f rotation;

    int width;
    int height;
    float focalLength;

    bool antialiasing = true;

    //Camera() = default;

    Ray getRay(int const x, int const y, Sampler &sampler) const
    {
        float dX = 0;
        float dY = 0;

        // Apply jitter for antialiasing
        if (antialiasing)
        {
            dX = sampler.nextFloat() - 0.5f;
            dY = sampler.nextFloat() - 0.5f;
        }

        Vec3f dir{
            ((float)x + dX - width / 2.0f),
            ((float)y + dY - height / 2.0f),
            focalLength};
        dir = rotate(dir, rotation);

        return Ray{position, dir};
    }
};

}

#endif //RAYTRACER_CAMERA_H

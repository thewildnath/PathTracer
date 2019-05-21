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

    bool antialiasing = true;

    float aperture = 0;
    float focalLength = 0.5;

    //Camera() = default;

    Vec3f getCentre() const
    {

    }

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
            (width + height) / 2};
        dir = rotate(dir, rotation);

        return Ray{position, dir};
    }

    Ray getLensRay(int const x, int const y, Sampler &sampler) const
    {
        Ray focalRay = getRay(x, y, sampler);
        Vec3f objective = focalRay(focalLength);

        Vec3f dPos{
            (sampler.nextFloat() - 0.5f) * aperture,
            (sampler.nextFloat() - 0.5f) * aperture,
            0.0f
        };

        Vec3f pos = position + rotate(dPos, rotation);;
        Vec3f dir = objective - pos;

        return Ray{pos, dir};
    }
};

}

#endif //RAYTRACER_CAMERA_H

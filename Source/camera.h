#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "ray.h"

namespace scg
{

class Camera
{
public:
    Vec3f position;
    //Vec3f rotation;

    int width;
    int height;
    float focalLength;

    Camera() = default;

    Ray getRay(int const x, int const y) const
    {
        Vec3f dir{
            ((float)x - width / 2),
            ((float)y - height / 2),
            focalLength};

        return Ray{position, dir};
    }
};

}

#endif //RAYTRACER_CAMERA_H

#ifndef RAYTRACER_ENUMS_H
#define RAYTRACER_ENUMS_H

enum SurfaceType
{
    Surface = 0,
    Volume = (1 << 0)
};

enum BSDFLobe
{
    Null = 0,
    Diffuse = (1 << 0),
    SpecularReflection = (1 << 1),
    SpecularTransmission = (1 << 2),
    Specular = SpecularReflection | SpecularTransmission
};

#endif //RAYTRACER_ENUMS_H

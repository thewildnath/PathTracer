#ifndef RAYTRACER_BSDF_LOBE_H
#define RAYTRACER_BSDF_LOBE_H

namespace scg
{

enum BSDFLobe
{
    Null = 0,
    Diffuse = (1 << 0),
    SpecularReflection = (1 << 1),
    SpecularTransmission = (1 << 2),
    Specular = SpecularReflection | SpecularTransmission
};

}

#endif //RAYTRACER_BSDF_LOBE_H

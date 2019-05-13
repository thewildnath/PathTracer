#ifndef RAYTRACER_SETTINGS_H
#define RAYTRACER_SETTINGS_H

#include "transferfunction.h"
#include "vector_type.h"

#include <utility>
#include <vector>

namespace scg
{

class Settings
{
public:
    Vec3f lightDir;

    float stepSize;
    float stepSizeWoodcock;

    float densityScale;

    float slice;

    int octreeLevels;

    TransferFunction transferFunction;

    std::vector<float> brackets;
    std::vector<float> maxOpacity;
    std::vector<float> minStepSize;

    int mask;
};

}

#endif //RAYTRACER_SETTINGS_H

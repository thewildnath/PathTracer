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
    int renderType;
    int minDepth;
    int maxDepth;
    float gamma;

    Vec3f backgroundLight;

    bool useBox;
    BoundingBox bb;

    float stepSize;
    float densityScale;
    float gradientFactor;

    int octreeLevels;

    TransferFunction transferFunction;

    std::vector<float> brackets;
    std::vector<float> maxOpacity;
    std::vector<float> minStepSize;

    int mask;
};

}

#endif //RAYTRACER_SETTINGS_H

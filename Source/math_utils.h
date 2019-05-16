#ifndef RAYTRACER_MATH_UTILS_H
#define RAYTRACER_MATH_UTILS_H

#include <cmath>

namespace scg
{

const float EPS = 1e-6;
//const float EPS = std::numeric_limits<float>::epsilon();
const float RAY_EPS = 1e-3;
const float INF = std::numeric_limits<float>::infinity();


inline float lerp(float value1, float value2, float weight)
{
    return value1 * (1.0f - weight) + value2 * weight;
}

template<typename T>
inline T clamp(T const& value, T const& low, T const& high)
{
    return value < low ? low : value > high ? high : value;
}

inline float toRadians(float degree)
{
    return degree * (float)M_PI / 180.0f;
}

inline float Fresnel(float iorI, float iorO, float VdotN, float sinSquaredThetaT)
{
    // Check for total internal reflection
    if (sinSquaredThetaT > 1.0f)
    {
        return 1.0f;
    }

    float cosThetaT = sqrtf(1.0f - sinSquaredThetaT);
    float R_perpendicular = (iorI * VdotN - iorO * cosThetaT) / (iorI * VdotN + iorO * cosThetaT);
    float R_parallel = (iorO * VdotN - iorI * cosThetaT) / (iorO * VdotN + iorI * cosThetaT);

    return 0.5f * (R_perpendicular * R_perpendicular + R_parallel * R_parallel);
}

inline float SinSquaredThetaT(float VdotN, float eta)
{
    return eta * eta * (1.0f - VdotN * VdotN);
}

}

#endif //RAYTRACER_MATH_UTILS_H

#ifndef RAYTRACER_MATH_UTILS_H
#define RAYTRACER_MATH_UTILS_H

#include <cmath>

namespace scg
{

const float EPS = 1e-6;
//const float EPS = std::numeric_limits<float>::epsilon();
const float INF = std::numeric_limits<float>::infinity();

template<typename T>
inline T clamp(T const& value, T const& low, T const& high)
{
    return value < low ? low : value > high ? high : value;
}

inline float toRadians(float degree)
{
    return degree * (float)M_PI / 180.0f;
}

}

#endif //RAYTRACER_MATH_UTILS_H

#ifndef RAYTRACER_MATH_UTILS_H
#define RAYTRACER_MATH_UTILS_H

template<typename T>
inline T clamp(T const& value, T const& low, T const& high)
{
    return value < low ? low : value > high ? high : value;
}

#endif //RAYTRACER_MATH_UTILS_H

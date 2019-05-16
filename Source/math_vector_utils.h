#ifndef RAYTRACER_MATH_VECTOR_UTILS_H
#define RAYTRACER_MATH_VECTOR_UTILS_H

#include "sampler.h"
#include "vector_type.h"

namespace scg
{

inline Vec3f reflect(Vec3f const& v, Vec3f const& normal) {
    return normal * 2.0f * dot(v, normal) - v;
}

inline Vec3f refract(const Vec3f &V, const Vec3f &N, float VdotN, float eta, float sinSquaredThetaT)
{
    return N * (eta * VdotN - sqrtf(1.0f - sinSquaredThetaT)) - V * eta;
}

inline void createCoordinateSystem(Vec3f const& N, Vec3f &Nt, Vec3f &Nb)
{
    if (std::fabs(N.x) > std::fabs(N.y))
        Nt = Vec3f(N.z, 0, -N.x) / std::sqrt(N.x * N.x + N.z * N.z);
    else
        Nt = Vec3f(0, -N.z, N.y) / std::sqrt(N.y * N.y + N.z * N.z);
    Nb = cross(N, Nt);
}

inline Vec3f uniformSampleHemisphere(float const& r1, float const& r2)
{
    // cos(theta) = u1 = y
    // cos^2(theta) + sin^2(theta) = 1 -> sin(theta) = srtf(1 - cos^2(theta))
    float sinTheta = sqrtf(1 - r1 * r1);
    float phi = 2 * (float)M_PI * r2;
    float x = sinTheta * cosf(phi);
    float z = sinTheta * sinf(phi);
    return Vec3f(x, r1, z);
}

inline Vec3f sampleHemisphere(Vec3f const& normal, Sampler& sampler)
{
    Vec3f Nt, Nb;
    createCoordinateSystem(normal, Nt, Nb);

    float r1 = sampler.nextFloat();
    float r2 = sampler.nextFloat();
    Vec3f sample = uniformSampleHemisphere(r1, r2);
    Vec3f nextDirection(
        sample.x * Nb.x + sample.y * normal.x + sample.z * Nt.x,
        sample.x * Nb.y + sample.y * normal.y + sample.z * Nt.y,
        sample.x * Nb.z + sample.y * normal.z + sample.z * Nt.z);

    return nextDirection;
}

inline Vec3f sampleSphere(Sampler &sampler)
{
    // Mathsy version
    /*
    double theta = 2 * M_PI * uniform01(generator);
    double phi = acos(1 - 2 * uniform01(generator));
    double x = sin(phi) * cos(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(phi);*/

    // Engineer version
    Vec3f point;
    float length;

    do
    {
        point.x = sampler.nextFloat() - 0.5f;
        point.y = sampler.nextFloat() - 0.5f;
        point.z = sampler.nextFloat() - 0.5f;
        length = point.length();
    } while (length <= EPS); // Make sure that we don't divide by 0

    point /= length;

    return point;
}

}

#endif //RAYTRACER_MATH_VECTOR_UTILS_H

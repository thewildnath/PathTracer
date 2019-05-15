#ifndef RAYTRACER_RAYMARCH_H
#define RAYTRACER_RAYMARCH_H

#include "intersection.h"
#include "ray.h"
#include "sampler.h"
#include "settings.h"
#include "vector_type.h"
#include "volume.h"

namespace scg
{

bool castRayWoodcock(Volume const& volume, Ray const& ray, Intersection &intersection, Settings const& settings, Sampler &sampler);

bool castRayWoodcockFast(Volume const& volume, Ray ray, Intersection &intersection, Settings const& settings, Sampler &sampler);

bool castRayWoodcockFast2(Volume const& volume, Ray ray, Intersection &intersection, Settings const& settings, Sampler &sampler);

Vec3f singleScatter(Volume const&, Ray const&, Settings const& settings, Sampler &sampler);

}

#endif //RAYTRACER_RAYMARCH_H

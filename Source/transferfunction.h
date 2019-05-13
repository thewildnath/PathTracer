#ifndef RAYTRACER_TRANSFERFUNCTION_H
#define RAYTRACER_TRANSFERFUNCTION_H

#include "math_utils.h"
#include "vector_type.h"

#include <iostream>
#include <algorithm>
#include <vector>

namespace scg
{

class Node
{
//private:
public:
    float intensity;
    float opacity;

    Vec3f colour;

    friend class TransferFunction;

public:
    Node(float intensity, float opacity, Vec3f const& colour):
        intensity(intensity), opacity(opacity), colour(colour) {};
};

// Nice code design here...
inline bool operator <(float const& intensity, Node const& node)
{
    return intensity < node.intensity;
}

class TransferFunction
{
private:
    std::vector<Node> nodes;

public:
    TransferFunction() = default;

    TransferFunction(std::vector<Node> const& nodes):
        nodes(nodes) {};

    inline Vec4f evaluate(float intensity) const
    {
        auto const& upper = std::upper_bound(nodes.begin(), nodes.end(), intensity);
        auto const& lower = upper - 1;

        float dx = (*upper).intensity - (*lower).intensity;
        float dist = (intensity - (*lower).intensity) / dx;

        Vec4f out = Vec4f(
            lerp((*lower).colour.x, (*upper).colour.x, dist),
            lerp((*lower).colour.y, (*upper).colour.y, dist),
            lerp((*lower).colour.z, (*upper).colour.z, dist),
            lerp((*lower).opacity, (*upper).opacity, dist));

        return out;
    }
};

}

#endif //RAYTRACER_TRANSFERFUNCTION_H

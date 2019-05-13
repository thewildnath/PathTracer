#include "raycast.h"

#include "boundingbox.h"
#include "octree.h"
#include "ray.h"
#include "settings.h"
#include "volume.h"
#include "vector_type.h"

#include <algorithm>
#include <iostream>
#include <stack>

namespace scg
{

float dT = 0.01;

struct State
{
    Octree const* node;
    float minT;
    float maxT;
    int nodesMask; // Intersection with child nodes

    State() = default;

    State(Octree const* node, float minT, float maxT)
    {
        this->node = node;
        this->minT = minT;
        this->maxT = maxT;
        this->nodesMask = 0;
    }
};

class ScatterEvent
{
public:
    bool isTrue;
    float t;

    ScatterEvent():
        isTrue(false), t(0) {};

    ScatterEvent(bool isTrue, float t):
        isTrue(isTrue), t(t) {};
};

ScatterEvent castRayWoodcock(Volume const& volume, Ray const& ray, Settings const& settings, Sampler &sampler)
{
    ScatterEvent scatterEvent;

    Vec3f color(0, 0, 0);

    BBIntersection bbIntersection;
    volume.octree.bb.getIntersection(ray, bbIntersection);

    if (!bbIntersection.valid)
    {
        return scatterEvent;
    }

    float minT = std::max(ray.minT, bbIntersection.nearT);
    float maxT = std::min(ray.maxT, bbIntersection.farT);

    minT += (-std::log(sampler.nextFloat())) * settings.stepSizeWoodcock;

    float invMaxDensity = 1.0f;

    while (minT <= maxT)
    {
        Vec3f pos = ray.origin + ray.direction * minT;

        float coef = volume.sampleVolume(pos);

        Vec4f out = settings.transferFunction.evaluate(coef);

        if (sampler.nextFloat() < out.w * invMaxDensity * settings.densityScale * settings.stepSizeWoodcock)
        {
            scatterEvent.isTrue = true;
            scatterEvent.t = minT;
            return scatterEvent;
        }

        minT += (-std::log(sampler.nextFloat())) * invMaxDensity * settings.stepSizeWoodcock;
    }

    return scatterEvent;
}

ScatterEvent castRayWoodcockFast(Volume const& volume, Ray ray, Settings const& settings, Sampler &sampler)
{
    ScatterEvent scatterEvent;

    Vec3f color(0, 0, 0);

    std::stack<State> st;

    BBIntersection bbIntersection;
    volume.octree.bb.getIntersection(ray, bbIntersection);
    if (bbIntersection.valid)
    {
        st.push(State(&volume.octree, bbIntersection.nearT, bbIntersection.farT));
    }

    ray.minT +=  (-std::log(sampler.nextFloat())) * settings.stepSizeWoodcock;

    while (!st.empty() && ray.minT <= ray.maxT)
    {
        State &state = st.top();
        Octree const* node = state.node;

        float minT = std::max(ray.minT, state.minT);
        float maxT = std::min(ray.maxT, state.maxT);

        // Ray was moved by a previous node
        if (minT > maxT)
        {
            st.pop();
            continue;
        }

        // Skip
        if (!(node->mask & settings.mask))
        {
            // Jump into next node
            ray.minT = maxT + dT;
            st.pop();
            continue;
        }

        // Continue 'recursively'
        if (!node->isLeaf)
        {
            // Find first child
            while(minT <= maxT)
            {
                Vec3f mid = node->bb.mid;
                Vec3f entry = ray(minT);
                Vec3f dist = entry - mid;

                bool sideX = dist.x >= 0;
                bool sideY = dist.y >= 0;
                bool sideZ = dist.z >= 0;

                int id = (sideX << 2) | (sideY << 1) | (sideZ);

                // We need to jump over
                if (state.nodesMask & (1 << id))
                {
                    minT += dT;
                    ray.minT = minT + dT;
                    continue;
                }

                node->nodes[id]->bb.getIntersection(ray, bbIntersection);
                state.nodesMask &= (1 << id);

                // We need to jump over
                if (!bbIntersection.valid)
                {
                    minT += dT;
                    ray.minT = minT + dT;
                    continue;
                }

                st.push(State(node->nodes[id], bbIntersection.nearT, bbIntersection.farT));
                break;
            }

            continue;
        }

        // Cast ray inside node
/*
        float maxOpacity = 0.0f;
        for (int i = 0; i < (int)settings.maxOpacity.size(); ++i)
        {
            if (node->mask & (1 << i) && settings.maxOpacity[i] > maxOpacity)
            {
                maxOpacity = settings.maxOpacity[i];
            }
        }
//*/
        float invMaxOpacity = 1.0f;// / maxOpacity;
        //float invMaxOpacityDensity = invMaxOpacity / settings.densityScale;

        while (minT <= maxT)
        {
            Vec3f pos = ray(minT);

            float coef = volume.sampleVolume(pos);

            Vec4f out = settings.transferFunction.evaluate(coef);

            if (sampler.nextFloat() < (out.w * settings.densityScale) * invMaxOpacity * settings.stepSizeWoodcock)
            {
                scatterEvent.isTrue = true;
                scatterEvent.t = minT;
                return scatterEvent;
            }

            minT += (-std::log(sampler.nextFloat())) * invMaxOpacity * settings.stepSizeWoodcock;
        }

        // Jump into next node
        ray.minT = minT;
        st.pop();
    }

    return scatterEvent;
}

Vec3f singleScatter(Volume const& volume, Ray const& ray, Settings const& settings, Sampler &sampler)
{
    ScatterEvent scatterEvent = castRayWoodcockFast(volume, ray, settings, sampler);

    if (!scatterEvent.isTrue)
    {
        return Vec3f(0, 0, 0); // Background
    }

    Vec3f pos = ray(scatterEvent.t);

    Vec3f normal = normalise(volume.getNormal(pos, 0.5f));

    float coef = volume.sampleVolume(pos);

    Vec4f out = settings.transferFunction.evaluate(coef);

    float light = 0.1f;

    if (true)
    {
        Ray lightRay(pos, -settings.lightDir);

        if (!castRayWoodcockFast(volume, lightRay, settings, sampler).isTrue)
        {
            light = std::max(light, dot(normal, settings.lightDir));
        }
    }

    //Vec3f reflected = glm::normalize(glm::reflect(ray.dir, normal));
    //float specularLow  = std::pow(glm::dot(-ray.dir, reflected), 10);
    //float specularHigh = std::pow(glm::dot(-ray.dir, reflected), 500);

    Vec3f color = (Vec3f(out.x, out.y,  out.z) * light * 1.0f);// + specularLow * 20.0f + specularHigh * 50.0f);
    return color / 255.0f;
}

}
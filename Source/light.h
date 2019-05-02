#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include "object.h"
#include "sampler.h"
#include "surfaceinteraction.h"
#include "vector_type.h"

#include <memory>

namespace scg
{

// Information regarding the illuminance of a light at a certain position
class LightHit
{
public:
    Vec3f colour;
    float intensity;

    Vec3f direction; // Normalised vector pointing to the source. Equal to the normal in case of AbstractLight.
    float distance;
};

// Light types
enum LightType {
    LightType_Abstract,
    LightType_Point,
    LightType_Directional,
    LightType_Object
};

// Base Light class
class Light
{
protected:
    Vec3f colour;
    float intensity;

public:
    Light(Vec3f const& colour, float intensity):
        colour(colour), intensity(intensity) {};

    virtual LightHit illuminate(SurfaceInteraction const&, Sampler &) const = 0;

    virtual Vec3f getEmittance(SurfaceInteraction const& interaction)
    {
        // Do not illuminate on the back side
        return colour * intensity * std::max(0.0f, dot(interaction.normal, interaction.outputDir));
    }

    virtual LightType getType() const = 0;
};

// An abstract light, assume it is located at the position where it is sampled
// Always returns maximum intensity
class AbstractLight : public Light
{
private:
    Vec3f colour;
    float intensity;

public:
    AbstractLight(Vec3f const& colour, float intensity):
        Light(colour, intensity) {};

    LightHit illuminate(SurfaceInteraction const& interaction, Sampler&) const override
    {
        LightHit lightHit;

        lightHit.colour = colour;
        lightHit.intensity = intensity;
        lightHit.direction = interaction.normal;

        return lightHit;
    }

    LightType getType() const override
    {
        return LightType_Abstract;
    }
};

// A source of light at a particular position with no shape/body
class PointLight : public Light
{
private:
    Vec3f position;

public:
    PointLight(Vec3f const& colour, float intensity, Vec3f const& position):
        Light(colour, intensity), position(position) {};

    LightHit illuminate(SurfaceInteraction const& interaction, Sampler&) const override
    {
        LightHit lightHit;

        lightHit.colour = colour;

        lightHit.direction = this->position - interaction.getSafePosition();
        lightHit.distance = lightHit.direction.length();
        lightHit.direction /= lightHit.distance;

        lightHit.intensity = intensity / (float)(4 * M_PI * lightHit.distance * lightHit.distance);

        return lightHit;
    }

    LightType getType() const override
    {
        return LightType_Point;
    }
};

// A source of light in a particular direction with no shape/body and position
class DirectionalLight : public Light
{
private:
    Vec3f direction;

public:
    DirectionalLight(Vec3f const& colour, float intensity, Vec3f const& direction):
        Light(colour, intensity), direction(normalise(direction)) {};

    LightHit illuminate(SurfaceInteraction const&, Sampler&) const override
    {
        LightHit lightHit;

        lightHit.colour = colour;

        lightHit.direction = -direction; // TODO: generate slightly random direction
        lightHit.distance = INF;

        lightHit.intensity = intensity;

        return lightHit;
    }

    LightType getType() const override
    {
        return LightType_Directional;
    }
};

class ObjectLight : public Light
{
private:
    std::shared_ptr<Object> object;

public:
    ObjectLight(Vec3f const& colour, float intensity, std::shared_ptr<Object> object):
        Light(colour, intensity), object(object) {};

    LightHit illuminate(SurfaceInteraction const& interaction, Sampler &sampler) const override
    {
        LightHit lightHit;

        SurfaceInteraction source = object->sampleSurface(sampler);

        lightHit.colour = colour;

        lightHit.direction = source.position - interaction.getSafePosition();
        lightHit.distance = lightHit.direction.length();
        lightHit.direction /= lightHit.distance;

        lightHit.intensity = intensity / (float)(4 * M_PI * lightHit.distance * lightHit.distance);
        lightHit.intensity = lightHit.intensity * std::max(0.0f, dot(source.normal, -lightHit.direction));

        return lightHit;
    }

    LightType getType() const override
    {
        return LightType_Object;
    }
};

}

#endif //RAYTRACER_LIGHT_H

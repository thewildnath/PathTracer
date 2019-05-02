#ifndef RAYTRACER_TEXTURE_H
#define RAYTRACER_TEXTURE_H

#include "vector_type.h"

namespace scg
{

class Texture
{
public:
    virtual Vec3f evaluate(Vec2f const&) const = 0;
};

class ColourTexture : public Texture
{
public:
    Vec3f colour;

    ColourTexture(Vec3f const& colour):
        colour(colour) {};

    Vec3f evaluate(Vec2f const&) const override
    {
        return colour;
    }
};

/*
class ImageTexture : public Texture
{
public:
    ImageTexture() {};

    Vec3f evaluate(Vec2f const&) const override
    {

    }
};
*/

}

#endif //RAYTRACER_TEXTURE_H

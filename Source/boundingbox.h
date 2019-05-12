#ifndef RAYTRACER_BOUNDINGBOX_H
#define RAYTRACER_BOUNDINGBOX_H

#include "ray.h"
#include "vector_type.h"

#include <limits>

namespace scg
{

class BBIntersection
{
public:
    BBIntersection()
    {
        this->SetInvalid();
    }

    void SetValid(float NearT)
    {
        this->valid		= true;
        this->nearT		= NearT;
    }

    void SetInvalid()
    {
        this->valid		= false;
        this->front		= true;
        this->nearT		= 0.0f;
        this->farT		= std::numeric_limits<float>::max();
    }

    BBIntersection& operator = (BBIntersection const& Other)
    {
        this->valid			= Other.valid;
        this->front			= Other.front;
        this->nearT			= Other.nearT;
        this->farT			= Other.farT;

        return *this;
    }

    bool	valid;
    bool	front;
    float	nearT;
    float	farT;
};

class BoundingBox
{
public:
    Vec3f min;
    Vec3f max;
    Vec3f mid;

    BoundingBox() {};

    BoundingBox(Vec3f const&, Vec3f const&);

    void getIntersection(Ray const& ray, BBIntersection &) const;
};

}

#endif //RAYTRACER_BOUNDINGBOX_H

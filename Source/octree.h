#ifndef RAYTRACER_OCTREE_H
#define RAYTRACER_OCTREE_H

#include "boundingbox.h"

namespace scg
{

class Octree
{
public:
    BoundingBox bb;
    Octree* nodes[8];
    bool isLeaf;

    int mask; // Mask for buckets inside

    Octree() = default;

    Octree(BoundingBox const&);
};

}

#endif //RAYTRACER_OCTREE_H

#include "octree.h"

#include "boundingbox.h"

namespace scg
{

Octree::Octree(BoundingBox const& bb)
{
    this->bb = bb;
}

}
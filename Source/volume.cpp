#include "volume.h"

#include "math_utils.h"
#include "settings.h"

namespace scg
{

Volume::Volume(int height, int width, int depth):
    height(height), width(width), depth(depth)
{
    //this->data = new int[depth, height, width];

    this->octree = Octree(
        BoundingBox(Vec3f(0 + V_EPS, 0 + V_EPS, 0 + V_EPS), Vec3f(230 - V_EPS, 220 - V_EPS, 135 - V_EPS))
    );
}

void buildOctree(Volume const& volume, Octree &octree, int levels, Settings const& settings)
{
    BoundingBox &bb = octree.bb;

    // Leaf
    if (levels == 0)
    {
        octree.isLeaf = true;

        for (int x = (int)std::round(bb.min.x - 1); x <= (int)std::round(bb.max.x + 1); ++x)
        {
            for (int y = (int)std::round(bb.min.y - 1); y <= (int)std::round(bb.max.y + 1); ++y)
            {
                for (int z = (int)std::round(bb.min.z - 1); z <= (int)std::round(bb.max.z + 1); ++z)
                {
                    int bracket = 0;
                    float coef = volume.sampleVolume(Vec3f(x, y, z));
                    while (settings.brackets[bracket + 1] <= coef)
                        ++bracket;

                    octree.mask |= (1 << bracket);
                }
            }
        }

        return;
    }

    octree.isLeaf = false;

    // Create 8 child nodes
    // 000
    octree.nodes[0] = new Octree(
        BoundingBox(Vec3f(bb.min.x, bb.min.y, bb.min.z), Vec3f(bb.mid.x, bb.mid.y, bb.mid.z))
    );
    buildOctree(volume, *octree.nodes[0], levels - 1, settings);
    // 001
    octree.nodes[1] = new Octree(
        BoundingBox(Vec3f(bb.min.x, bb.min.y, bb.mid.z), Vec3f(bb.mid.x, bb.mid.y, bb.max.z))
    );
    buildOctree(volume, *octree.nodes[1], levels - 1, settings);
    // 010
    octree.nodes[2] = new Octree(
        BoundingBox(Vec3f(bb.min.x, bb.mid.y, bb.min.z), Vec3f(bb.mid.x, bb.max.y, bb.mid.z))
    );
    buildOctree(volume, *octree.nodes[2], levels - 1, settings);
    // 011
    octree.nodes[3] = new Octree(
        BoundingBox(Vec3f(bb.min.x, bb.mid.y, bb.mid.z), Vec3f(bb.mid.x, bb.max.y, bb.max.z))
    );
    buildOctree(volume, *octree.nodes[3], levels - 1, settings);
    // 100
    octree.nodes[4] = new Octree(
        BoundingBox(Vec3f(bb.mid.x, bb.min.y, bb.min.z), Vec3f(bb.max.x, bb.mid.y, bb.mid.z))
    );
    buildOctree(volume, *octree.nodes[4], levels - 1, settings);
    // 101
    octree.nodes[5] = new Octree(
        BoundingBox(Vec3f(bb.mid.x, bb.min.y, bb.mid.z), Vec3f(bb.max.x, bb.mid.y, bb.max.z))
    );
    buildOctree(volume, *octree.nodes[5], levels - 1, settings);
    // 110
    octree.nodes[6] = new Octree(
        BoundingBox(Vec3f(bb.mid.x, bb.mid.y, bb.min.z), Vec3f(bb.max.x, bb.max.y, bb.mid.z))
    );
    buildOctree(volume, *octree.nodes[6], levels - 1, settings);
    // 111
    octree.nodes[7] = new Octree(
        BoundingBox(Vec3f(bb.mid.x, bb.mid.y, bb.mid.z), Vec3f(bb.max.x, bb.max.y, bb.max.z))
    );
    buildOctree(volume, *octree.nodes[7], levels - 1, settings);

    int maskAll = octree.nodes[0]->mask;
    for (int i = 0; i < 8; ++i)
    {
        octree.mask |= octree.nodes[i]->mask;
        maskAll &= octree.nodes[i]->mask;
    }

    if (octree.mask == maskAll)
    {
        // TODO: free memory!
        octree.isLeaf = true;
    }
}

}
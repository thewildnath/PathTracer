#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "light.h"
#include "material.h"
#include "object.h"

#include <vector>

namespace scg
{

class Scene
{
public:
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Object>> objects;
};

}

#endif //RAYTRACER_SCENE_H

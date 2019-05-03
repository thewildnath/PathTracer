#include "utils.h"

#include "geometry.h"
#include "material.h"
#include "scene.h"
#include "texture.h"
#include "triangle.h"
#include "vector_type.h"

#include <vector>

namespace scg
{

Scene LoadTestModel()
{
    // Defines colours:
    ColourTexture redTexture(Vec3f(0.75f, 0.15f, 0.15f));
    ColourTexture yellowTexture(Vec3f(0.75f, 0.75f, 0.15f));
    ColourTexture greenTexture(Vec3f(0.15f, 0.75f, 0.15f));
    ColourTexture cyanTexture(Vec3f(0.15f, 0.75f, 0.75f));
    ColourTexture blueTexture(Vec3f(0.15f, 0.15f, 0.75f));
    ColourTexture purpleTexture(Vec3f(0.75f, 0.15f, 0.75f));
    ColourTexture whiteTexture(Vec3f(0.75f, 0.75f, 0.75f));

    Lambert red(std::make_shared<ColourTexture>(redTexture));
    Lambert yellow(std::make_shared<ColourTexture>(yellowTexture));
    Lambert green(std::make_shared<ColourTexture>(greenTexture));
    Lambert cyan(std::make_shared<ColourTexture>(cyanTexture));
    Lambert blue(std::make_shared<ColourTexture>(blueTexture));
    Lambert purple(std::make_shared<ColourTexture>(purpleTexture));
    Lambert white(std::make_shared<ColourTexture>(whiteTexture));

    std::vector<std::shared_ptr<Material>> materials;
    materials.push_back(std::make_shared<Lambert>(red));    // 0
    materials.push_back(std::make_shared<Lambert>(yellow)); // 1
    materials.push_back(std::make_shared<Lambert>(green));  // 2
    materials.push_back(std::make_shared<Lambert>(cyan));   // 3
    materials.push_back(std::make_shared<Lambert>(blue));   // 4
    materials.push_back(std::make_shared<Lambert>(purple)); // 5
    materials.push_back(std::make_shared<Lambert>(white));  // 6

    std::vector<Triangle> triangles;
    triangles.reserve(5 * 2 * 3);

    // ---------------------------------------------------------------------------
    // Room

    float L = 555;            // Length of Cornell Box side.

    Vec3f A(L, 0, 0);
    Vec3f B(0, 0, 0);
    Vec3f C(L, 0, L);
    Vec3f D(0, 0, L);

    Vec3f E(L, L, 0);
    Vec3f F(0, L, 0);
    Vec3f G(L, L, L);
    Vec3f H(0, L, L);

    // Floor:
    triangles.emplace_back(Triangle(C, B, A, 6));//2));
    triangles.emplace_back(Triangle(C, D, B, 6));//2));

    // Left wall
    triangles.emplace_back(Triangle(A, E, C, 2));//5));
    triangles.emplace_back(Triangle(C, E, G, 2));//5));

    // Right wall
    triangles.emplace_back(Triangle(F, B, D, 0));//1));
    triangles.emplace_back(Triangle(H, F, D, 0));//1));

    // Ceiling
    triangles.emplace_back(Triangle(E, F, G, 6));//3));
    triangles.emplace_back(Triangle(F, H, G, 6));//3));

    // Back wall
    triangles.emplace_back(Triangle(G, D, C, 6));//6));
    triangles.emplace_back(Triangle(G, H, D, 6));//6));

    // ---------------------------------------------------------------------------
    // Short block

    A = Vec3f(290, 0, 114);
    B = Vec3f(130, 0, 65);
    C = Vec3f(240, 0, 272);
    D = Vec3f(82, 0, 225);

    E = Vec3f(290, 165, 114);
    F = Vec3f(130, 165, 65);
    G = Vec3f(240, 165, 272);
    H = Vec3f(82, 165, 225);

    // Front
    triangles.emplace_back(Triangle(E, B, A, 6));//0));
    triangles.emplace_back(Triangle(E, F, B, 6));//0));

    // Front
    triangles.emplace_back(Triangle(F, D, B, 6));//0));
    triangles.emplace_back(Triangle(F, H, D, 6));//0));

    // BACK
    triangles.emplace_back(Triangle(H, C, D, 6));//0));
    triangles.emplace_back(Triangle(H, G, C, 6));//0));

    // LEFT
    triangles.emplace_back(Triangle(G, E, C, 6));//0));
    triangles.emplace_back(Triangle(E, A, C, 6));//0));

    // TOP
    triangles.emplace_back(Triangle(G, F, E, 6));//0));
    triangles.emplace_back(Triangle(G, H, F, 6));//0));

    // ---------------------------------------------------------------------------
    // Tall block

    A = Vec3f(423, 0, 247);
    B = Vec3f(265, 0, 296);
    C = Vec3f(472, 0, 406);
    D = Vec3f(314, 0, 456);

    E = Vec3f(423, 330, 247);
    F = Vec3f(265, 330, 296);
    G = Vec3f(472, 330, 406);
    H = Vec3f(314, 330, 456);

    // Front
    triangles.emplace_back(Triangle(E, B, A, 4));
    triangles.emplace_back(Triangle(E, F, B, 4));

    // Front
    triangles.emplace_back(Triangle(F, D, B, 4));
    triangles.emplace_back(Triangle(F, H, D, 4));

    // BACK
    triangles.emplace_back(Triangle(H, C, D, 4));
    triangles.emplace_back(Triangle(H, G, C, 4));

    // LEFT
    triangles.emplace_back(Triangle(G, E, C, 4));
    triangles.emplace_back(Triangle(E, A, C, 4));

    // TOP
    triangles.emplace_back(Triangle(G, F, E, 4));
    triangles.emplace_back(Triangle(G, H, F, 4));

    // ----------------------------------------------
    // Scale to the volume [-1,1]^3

    for (auto& triangle : triangles)
    {
        triangle.v0 *= 2 / L;
        triangle.v1 *= 2 / L;
        triangle.v2 *= 2 / L;

        triangle.v0 -= Vec3f(1, 1, 1);
        triangle.v1 -= Vec3f(1, 1, 1);
        triangle.v2 -= Vec3f(1, 1, 1);

        triangle.v0.x *= -1;
        triangle.v1.x *= -1;
        triangle.v2.x *= -1;

        triangle.v0.y *= -1;
        triangle.v1.y *= -1;
        triangle.v2.y *= -1;

        triangle.ComputeNormal();
    }

    Scene scene;
    scene.materials = materials;
    scene.objects.push_back(std::make_shared<Object>(Object{
        {0, 0, 0},
        std::make_shared<Mesh>(Mesh(triangles))
    }));


    // Extra

    // Point lights
    //scene.lights.emplace_back(std::make_shared<scg::PointLight>(scg::PointLight{{1.0f, 1.0f, 1.0f}, 40, {0, -0.75, 0}}));
    //scene.lights.emplace_back(std::make_shared<scg::PointLight>(scg::PointLight{{1.0f, 1.0f, 1.0f}, 20, {-0.5, -0.75, 0}}));
    //scene.lights.emplace_back(std::make_shared<scg::PointLight>(scg::PointLight{{1.0f, 1.0f, 1.0f}, 20, {0.5, -0.75, 0}}));
    // Directional lights
    //scene.lights.emplace_back(std::make_shared<scg::DirectionalLight>(scg::DirectionalLight{{1.0f, 1.0f, 1.0f}, 1, {0.2, 0.5, 0.5}}));

    size_t index = scene.materials.size();
    std::shared_ptr<scg::ColourTexture> texture = std::make_shared<scg::ColourTexture>(scg::ColourTexture{
        {1.0f, 1.0f, 1.0f}
    });
//*
    // Ceiling light

    float eL = 1;
    Vec3f eE(eL / 2, 0, -eL / 2);
    Vec3f eF(-eL / 2, 0, -eL / 2);
    Vec3f eG(eL / 2, 0, eL / 2);
    Vec3f eH(-eL / 2, 0, eL / 2);
    std::vector<scg::Triangle> etriangles{
        scg::Triangle(eG, eF, eE, index),
        scg::Triangle(eG, eH, eF, index)
    };
    std::shared_ptr<scg::Object> objectPtr = std::make_shared<scg::Object>(scg::Object{
        { 0, -0.99, 0},
        std::make_shared<scg::Mesh>(etriangles)
    });
//    std::shared_ptr<scg::Object> objectPtr = std::make_shared<scg::Object>(scg::Object{
//        { 0, -0.7, 0},
//        std::make_shared<scg::Sphere>(0.2, index)
//    });
    scene.objects.emplace_back(objectPtr);
    std::shared_ptr<scg::ObjectLight> lightPtr = std::make_shared<scg::ObjectLight>(scg::ObjectLight{
        {1.0f, 1.0f, 1.0f}, 20,
        objectPtr
    });
    scene.lights.emplace_back(lightPtr);
    scene.materials.emplace_back(std::make_shared<scg::Lambert>(scg::Lambert{texture, lightPtr}));
//*/
    scene.materials.emplace_back(std::make_shared<scg::Mirror>(scg::Mirror{texture}));
    index = scene.materials.size() - 1;
    std::shared_ptr<scg::Object> object2Ptr = std::make_shared<scg::Object>(scg::Object{
        { 0.6, -0.15, 0},
        std::make_shared<scg::Sphere>(0.3, index)
    });
    scene.objects.emplace_back(object2Ptr);

    return scene;
}

}
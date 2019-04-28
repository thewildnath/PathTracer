#include "utils.h"

#include "geometry.h"
#include "material.h"
#include "scene.h"
#include "triangle.h"
#include "vector_type.h"

#include <vector>

namespace scg
{

Scene LoadTestModel()
{
    // Defines colours:
    Material red(Vec3f(0.75f, 0.15f, 0.15f));
    Material yellow(Vec3f(0.75f, 0.75f, 0.15f));
    Material green(Vec3f(0.15f, 0.75f, 0.15f));
    Material cyan(Vec3f(0.15f, 0.75f, 0.75f));
    Material blue(Vec3f(0.15f, 0.15f, 0.75f));
    Material purple(Vec3f(0.75f, 0.15f, 0.75f));
    Material white(Vec3f(0.75f, 0.75f, 0.75f));

    std::vector<Material> materials;
    materials.push_back(red);    // 0
    materials.push_back(yellow); // 1
    materials.push_back(green);  // 2
    materials.push_back(cyan);   // 3
    materials.push_back(blue);   // 4
    materials.push_back(purple); // 5
    materials.push_back(white);  // 6

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
    triangles.emplace_back(Triangle(C, B, A, 2));
    triangles.emplace_back(Triangle(C, D, B, 2));

    // Left wall
    triangles.emplace_back(Triangle(A, E, C, 5));
    triangles.emplace_back(Triangle(C, E, G, 5));

    // Right wall
    triangles.emplace_back(Triangle(F, B, D, 1));
    triangles.emplace_back(Triangle(H, F, D, 1));

    // Ceiling
    triangles.emplace_back(Triangle(E, F, G, 3));
    triangles.emplace_back(Triangle(F, H, G, 3));

    // Back wall
    triangles.emplace_back(Triangle(G, D, C, 6));
    triangles.emplace_back(Triangle(G, H, D, 6));

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
    triangles.emplace_back(Triangle(E, B, A, 0));
    triangles.emplace_back(Triangle(E, F, B, 0));

    // Front
    triangles.emplace_back(Triangle(F, D, B, 0));
    triangles.emplace_back(Triangle(F, H, D, 0));

    // BACK
    triangles.emplace_back(Triangle(H, C, D, 0));
    triangles.emplace_back(Triangle(H, G, C, 0));

    // LEFT
    triangles.emplace_back(Triangle(G, E, C, 0));
    triangles.emplace_back(Triangle(E, A, C, 0));

    // TOP
    triangles.emplace_back(Triangle(G, F, E, 0));
    triangles.emplace_back(Triangle(G, H, F, 0));

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

        triangle.v0.w = 1.0;
        triangle.v1.w = 1.0;
        triangle.v2.w = 1.0;

        triangle.ComputeNormal();
    }

    Scene scene;
    scene.materials = materials;
    scene.objects.push_back(scg::Object{
        {0, 0, 0},
        std::make_shared<scg::Mesh>(scg::Mesh(triangles))
    });

    return scene;
}

}
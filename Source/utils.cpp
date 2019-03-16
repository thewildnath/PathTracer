#include "utils.h"

#include "triangle.h"
#include "vector_type.h"

#include <vector>

namespace scg
{

void LoadTestModel(std::vector <Triangle> &triangles)
{
    // Defines colors:
    Vec3f red(0.75f, 0.15f, 0.15f);
    Vec3f yellow(0.75f, 0.75f, 0.15f);
    Vec3f green(0.15f, 0.75f, 0.15f);
    Vec3f cyan(0.15f, 0.75f, 0.75f);
    Vec3f blue(0.15f, 0.15f, 0.75f);
    Vec3f purple(0.75f, 0.15f, 0.75f);
    Vec3f white(0.75f, 0.75f, 0.75f);

    triangles.clear();
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
    triangles.emplace_back(Triangle(C, B, A, green));
    triangles.emplace_back(Triangle(C, D, B, green));

    // Left wall
    triangles.emplace_back(Triangle(A, E, C, purple));
    triangles.emplace_back(Triangle(C, E, G, purple));

    // Right wall
    triangles.emplace_back(Triangle(F, B, D, yellow));
    triangles.emplace_back(Triangle(H, F, D, yellow));

    // Ceiling
    triangles.emplace_back(Triangle(E, F, G, cyan));
    triangles.emplace_back(Triangle(F, H, G, cyan));

    // Back wall
    triangles.emplace_back(Triangle(G, D, C, white));
    triangles.emplace_back(Triangle(G, H, D, white));

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
    triangles.emplace_back(Triangle(E, B, A, red));
    triangles.emplace_back(Triangle(E, F, B, red));

    // Front
    triangles.emplace_back(Triangle(F, D, B, red));
    triangles.emplace_back(Triangle(F, H, D, red));

    // BACK
    triangles.emplace_back(Triangle(H, C, D, red));
    triangles.emplace_back(Triangle(H, G, C, red));

    // LEFT
    triangles.emplace_back(Triangle(G, E, C, red));
    triangles.emplace_back(Triangle(E, A, C, red));

    // TOP
    triangles.emplace_back(Triangle(G, F, E, red));
    triangles.emplace_back(Triangle(G, H, F, red));

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
    triangles.emplace_back(Triangle(E, B, A, blue));
    triangles.emplace_back(Triangle(E, F, B, blue));

    // Front
    triangles.emplace_back(Triangle(F, D, B, blue));
    triangles.emplace_back(Triangle(F, H, D, blue));

    // BACK
    triangles.emplace_back(Triangle(H, C, D, blue));
    triangles.emplace_back(Triangle(H, G, C, blue));

    // LEFT
    triangles.emplace_back(Triangle(G, E, C, blue));
    triangles.emplace_back(Triangle(E, A, C, blue));

    // TOP
    triangles.emplace_back(Triangle(G, F, E, blue));
    triangles.emplace_back(Triangle(G, H, F, blue));


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
}

}
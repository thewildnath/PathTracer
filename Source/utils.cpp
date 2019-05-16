#include "utils.h"

#include "geometry.h"
#include "material.h"
#include "settings.h"
#include "scene.h"
#include "texture.h"
#include "triangle.h"
#include "vector_type.h"

#include "tinytiffreader.h"

#include <fstream>
#include <vector>

namespace scg
{

Settings loadSettings()
{
    Settings settings;

    settings.renderType = 2;
    settings.lightDir = scg::normalise(scg::Vec3f(1.0f, 0.5f, 1.0f));
    settings.stepSize = 0.1f;
    settings.octreeLevels = 5;
    settings.brackets = std::vector<float>{
        0, 1000, 1300, 1500, 1750, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2850, 3000, 3250, 3500, 99999 // 1 less than TF!
    };
    settings.maxOpacity.resize(settings.brackets.size() - 1);
    settings.minStepSize.resize(settings.brackets.size() - 1);

    return settings;
}

void loadTransferFunction(Settings &settings)
{
    std::ifstream fin;
    fin.open("transfer.txt");

    std::vector<scg::Node> nodes;

    float x, a, r, g, b;

    fin >> settings.densityScale;
    fin >> settings.gradientFactor;
    fin >> settings.stepSize;

    while (fin >> x >> a >> r >> g >> b)
    {
        nodes.emplace_back(scg::Node{x, a, Vec3f{r, g, b} / 255.0f});
    }

    settings.transferFunction = scg::TransferFunction(nodes);
//*
    for (int i = 0; i < (int)settings.minStepSize.size(); ++i)
    {
        settings.maxOpacity[i] = 0;
        settings.minStepSize[i] = 0;
    }
//*/
    settings.mask = 0;
    for (size_t i = 0; i < nodes.size() - 1; ++i)
    {
        if (nodes[i].opacity > 0 || nodes[i + 1].opacity > 0)
        {
            for (int bracket = 0; bracket < (int)settings.brackets.size() - 1; ++bracket)
            {
                float minX = std::fmaxf(nodes[i].intensity, settings.brackets[bracket]);
                float maxX = std::fminf(nodes[i + 1].intensity, settings.brackets[bracket + 1]);

                if (minX < maxX)
                {
                    settings.mask |= (1 << bracket);
                    //*
                    float maxOpacity = std::fmaxf(settings.transferFunction.evaluate(minX).w, settings.transferFunction.evaluate(maxX).w);
                    if (maxOpacity > settings.maxOpacity[bracket])
                    {
                        settings.maxOpacity[bracket] = maxOpacity;
                    }
                    //*/
                }
            }
        }
    }
//*
    for (int i = 0; i < (int)settings.minStepSize.size(); ++i)
    {
        settings.minStepSize[i] =
            1.0f * settings.maxOpacity[i] + 0.1f * (1 - settings.maxOpacity[i]);
    }
//*/
}

void loadBrain(scg::Volume& volume, scg::Volume& temp, Scene &scene, scg::Settings const& settings)
{
    char filename[50] = "../data/StanfordBrain/mrbrain-16bit000.tif";
    for (int x = 0; x < 99; ++x)
    {
        sprintf(filename + 35, "%03d.tif", x + 1);
        std::cout << "Loading: " << filename << std::endl;

        TinyTIFFReaderFile* tiffr = TinyTIFFReader_open(filename);
        if (!tiffr)
        {
            std::cout<<"ERROR reading (not existent, not accessible or no TIFF file)\n";
        }
        else
        {
            int width = TinyTIFFReader_getWidth(tiffr);
            int height = TinyTIFFReader_getHeight(tiffr);
            uint16_t* image = (uint16_t*)calloc((size_t)width * height, sizeof(uint16_t));
            TinyTIFFReader_getSampleData(tiffr, image, 0);

            for (int y = 0; y < height; ++y)
            {
                for (int z = 0; z < width; ++z)
                {
                    temp.data[z][y][x] = image[y * width + z];
                }
            }

            free(image);
        }
        TinyTIFFReader_close(tiffr);
    }
    
    for (int x = 0; x < volume.width; ++x)
    {
        for (int y = 0; y < volume.height; ++y)
        {
            for (int z = 0; z < volume.height; ++z)
            {
                volume.data[z][y][x] = temp.sampleVolume(Vec3f(z, y, x / 1.3f));
            }
        }
    }

    buildOctree(volume, volume.octree, settings.octreeLevels, settings);

    scene.volume = std::make_shared<Volume>(volume);
    scene.volumePos = Vec3f{-135, -141, -75};

    std::cout << "Done loadBrain." << std::endl;

    // Point lights
    //scene.lights.emplace_back(std::make_shared<scg::PointLight>(scg::PointLight{{1.0f, 1.0f, 1.0f}, 20, {0.0f, -0.75f, 0.0f}}));
    // Directional lights
    //scene.lights.emplace_back(std::make_shared<scg::DirectionalLight>(scg::DirectionalLight{{1.0f, 1.0f, 1.0f}, 1, {1.0f, 0.5f, 1.0f}}));
}

Scene loadTestModel(float size)
{
    // Defines colours:
    std::shared_ptr<ColourTexture> redTexture = std::make_shared<ColourTexture>(Vec3f(0.75f, 0.15f, 0.15f));
    std::shared_ptr<ColourTexture> yellowTexture = std::make_shared<ColourTexture>(Vec3f(0.75f, 0.75f, 0.15f));
    std::shared_ptr<ColourTexture> greenTexture = std::make_shared<ColourTexture>(Vec3f(0.15f, 0.75f, 0.15f));
    std::shared_ptr<ColourTexture> cyanTexture = std::make_shared<ColourTexture>(Vec3f(0.15f, 0.75f, 0.75f));
    std::shared_ptr<ColourTexture> blueTexture = std::make_shared<ColourTexture>(Vec3f(0.15f, 0.15f, 0.75f));
    std::shared_ptr<ColourTexture> purpleTexture = std::make_shared<ColourTexture>(Vec3f(0.75f, 0.15f, 0.75f));
    std::shared_ptr<ColourTexture> whiteTexture = std::make_shared<ColourTexture>(Vec3f(0.75f, 0.75f, 0.75f));

    std::shared_ptr<Lambert> red = std::make_shared<Lambert>(redTexture);
    std::shared_ptr<Lambert> yellow = std::make_shared<Lambert>(yellowTexture);
    std::shared_ptr<Lambert> green = std::make_shared<Lambert>(greenTexture);
    std::shared_ptr<Lambert> cyan = std::make_shared<Lambert>(cyanTexture);
    std::shared_ptr<Lambert> blue = std::make_shared<Lambert>(blueTexture);
    std::shared_ptr<Lambert> purple = std::make_shared<Lambert>(purpleTexture);
    std::shared_ptr<Lambert> white = std::make_shared<Lambert>(whiteTexture);

    std::vector<std::shared_ptr<Material>> materials;
    materials.push_back(red);    // 0
    materials.push_back(yellow); // 1
    materials.push_back(green);  // 2
    materials.push_back(cyan);   // 3
    materials.push_back(blue);   // 4
    materials.push_back(purple); // 5
    materials.push_back(white);  // 6
    materials.push_back(std::make_shared<Mirror>(whiteTexture)); // 7
    materials.push_back(std::make_shared<SpecularDielectric>(whiteTexture, 1.52f)); // 8
    materials.push_back(std::make_shared<DiffuseDielectric>(whiteTexture, 1.52f)); // 9

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
    triangles.emplace_back(Triangle(C, B, A, 4));//2));
    triangles.emplace_back(Triangle(C, D, B, 4));//2));

    // Left wall
    triangles.emplace_back(Triangle(A, E, C, 0));//5));
    triangles.emplace_back(Triangle(C, E, G, 0));//5));

    // Right wall
    triangles.emplace_back(Triangle(F, B, D, 2));//1));
    triangles.emplace_back(Triangle(H, F, D, 2));//1));

    // Ceiling
    triangles.emplace_back(Triangle(E, F, G, 6));//3));
    triangles.emplace_back(Triangle(F, H, G, 6));//3));

    // Back wall
    triangles.emplace_back(Triangle(G, D, C, 6));//6));
    triangles.emplace_back(Triangle(G, H, D, 6));//6));

    // ---------------------------------------------------------------------------
    // Short block
/*
    A = Vec3f(290, 0, 114);
    B = Vec3f(130, 0, 65);
    C = Vec3f(240, 0, 272);
    D = Vec3f(82, 0, 225);

    E = Vec3f(290, 165, 114);
    F = Vec3f(130, 165, 65);
    G = Vec3f(240, 165, 272);
    H = Vec3f(82, 165, 225);

    // Front
    triangles.emplace_back(Triangle(E, B, A, 8));//0));
    triangles.emplace_back(Triangle(E, F, B, 8));//0));

    // Front
    triangles.emplace_back(Triangle(F, D, B, 8));//0));
    triangles.emplace_back(Triangle(F, H, D, 8));//0));

    // BACK
    triangles.emplace_back(Triangle(H, C, D, 8));//0));
    triangles.emplace_back(Triangle(H, G, C, 8));//0));

    // LEFT
    triangles.emplace_back(Triangle(G, E, C, 8));//0));
    triangles.emplace_back(Triangle(E, A, C, 8));//0));

    // TOP
    triangles.emplace_back(Triangle(G, F, E, 8));//0));
    triangles.emplace_back(Triangle(G, H, F, 8));//0));
//*/

    // ---------------------------------------------------------------------------
    // Tall block
/*
    A = Vec3f(423, 0, 247);
    B = Vec3f(265, 0, 296);
    C = Vec3f(472, 0, 406);
    D = Vec3f(314, 0, 456);

    E = Vec3f(423, 330, 247);
    F = Vec3f(265, 330, 296);
    G = Vec3f(472, 330, 406);
    H = Vec3f(314, 330, 456);

    // Front
    triangles.emplace_back(Triangle(E, B, A, 6));
    triangles.emplace_back(Triangle(E, F, B, 6));

    // Front
    triangles.emplace_back(Triangle(F, D, B, 6));
    triangles.emplace_back(Triangle(F, H, D, 6));

    // BACK
    triangles.emplace_back(Triangle(H, C, D, 6));
    triangles.emplace_back(Triangle(H, G, C, 6));

    // LEFT
    triangles.emplace_back(Triangle(G, E, C, 6));
    triangles.emplace_back(Triangle(E, A, C, 6));

    // TOP
    triangles.emplace_back(Triangle(G, F, E, 6));
    triangles.emplace_back(Triangle(G, H, F, 6));
//*/

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

        triangle.v0 *= size;
        triangle.v1 *= size;
        triangle.v2 *= size;

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
    scene.lights.emplace_back(std::make_shared<scg::PointLight>(scg::PointLight{{1.0f, 1.0f, 1.0f}, 40 * size * size, Vec3f{0.0f, -0.75f, 0.0f} * size}));
    //scene.lights.emplace_back(std::make_shared<scg::PointLight>(scg::PointLight{{1.0f, 1.0f, 1.0f}, 10 * size, Vec3f{-0.5f, -0.75f, 0.0f} * size}));
    //scene.lights.emplace_back(std::make_shared<scg::PointLight>(scg::PointLight{{1.0f, 1.0f, 1.0f}, 10 * size, Vec3f{0.5f, -0.75f, 0.0f} * size}));
    // Directional lights
    //scene.lights.emplace_back(std::make_shared<scg::DirectionalLight>(scg::DirectionalLight{{1.0f, 1.0f, 1.0f}, 10, {0.2, 0.5, 0.5}}));

    size_t index = scene.materials.size();
    std::shared_ptr<scg::ColourTexture> texture = std::make_shared<scg::ColourTexture>(scg::ColourTexture{
        {1.0f, 1.0f, 1.0f}
    });

/*
    // Ceiling light
    float eL = 0.5f * size;
    Vec3f eE(eL / 2, 0, -eL / 2);
    Vec3f eF(-eL / 2, 0, -eL / 2);
    Vec3f eG(eL / 2, 0, eL / 2);
    Vec3f eH(-eL / 2, 0, eL / 2);
    std::vector<scg::Triangle> etriangles{
        scg::Triangle(eG, eF, eE, index),
        scg::Triangle(eG, eH, eF, index)
    };
    std::shared_ptr<scg::Object> objectPtr = std::make_shared<scg::Object>(scg::Object{
        Vec3f{ 0, -0.99f, 0} * size,
        std::make_shared<scg::Mesh>(etriangles)
    });
//    std::shared_ptr<scg::Object> objectPtr = std::make_shared<scg::Object>(scg::Object{
//        Vec3f{ 0, -0.7, 0} * size,
//        std::make_shared<scg::Sphere>(0.2 * size, index)
//    });
    scene.objects.emplace_back(objectPtr);
    std::shared_ptr<scg::ObjectLight> lightPtr = std::make_shared<scg::ObjectLight>(scg::ObjectLight{
        Vec3f{1.0f, 1.0f, 1.0f}, 20 * size * size,
        objectPtr
    });
    scene.lights.emplace_back(lightPtr);
    scene.materials.emplace_back(std::make_shared<scg::Lambert>(scg::Lambert{texture, lightPtr}));
//*/
//*
    std::shared_ptr<scg::Object> sphere = std::make_shared<scg::Object>(scg::Object{
        Vec3f{ -0.4, 0.1, 0.2} * size,
        std::make_shared<scg::Sphere>(0.35 * size, 6)
    });
    scene.objects.emplace_back(sphere);
    std::shared_ptr<scg::Object> sphere2 = std::make_shared<scg::Object>(scg::Object{
        Vec3f{ 0.5, 0.55, -0.2} * size,
        std::make_shared<scg::Sphere>(0.4 * size, 6)
    });
    scene.objects.emplace_back(sphere2);
//*/
    return scene;
}

}
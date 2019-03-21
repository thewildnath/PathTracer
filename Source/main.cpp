#include "camera.h"
#include "object.h"
#include "ray.h"
#include "raytrace.h"
#include "SDLauxiliary.h"
#include "triangle.h"
#include "utils.h"
#include "vector_type.h"

#include <SDL.h>

#include <iostream>
#include <cstdint>
#include <cstring>

#define RES 400
#define SCREEN_WIDTH  RES
#define SCREEN_HEIGHT RES
#define FOCAL_LENGTH  RES
#define FULLSCREEN_MODE false

#undef main // Bloody hell, hope it doesn't come back and haunt me

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */
bool Update();
void Draw(screen *screen);

scg::Camera camera{
    scg::Vec3f(0, 0, -3.2),
    scg::Vec3f(0, 0, 0),
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    FOCAL_LENGTH};

std::vector<scg::Object> objects;

int main(int argc, char *argv[])
{
    screen *screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE);

    scg::Mesh mesh;
    scg::LoadTestModel(mesh.triangles);

    objects.emplace_back(scg::Object(
        scg::Vec3f(0, 0, 0),
        scg::Material({1, 1, 1}),
        *(new scg::Mesh(mesh.triangles)))); // TODO: why doesn't inplace work?
        //mesh));
    objects.emplace_back(scg::Object(
        scg::Vec3f(0, 0, 0),
        scg::Material({1, 1, 1}),
        *(new scg::Sphere(0.5f))));
    objects[1].material.emission = 10000;
    /*objects.emplace_back(scg::Object(
        scg::Vec3f(1, 0.5f, 2),
        scg::Material({1, 0, 0}),
        *(new scg::Sphere(1))));*/

    while (Update())
    {
        Draw(screen);
        SDL_Renderframe(screen);
    }

    SDL_SaveImage(screen, "screenshot.bmp");

    KillSDL(screen);
    return 0;
}

/*Place your drawing here*/
void Draw(screen *screen)
{
    /* Clear buffer */
    memset(screen->buffer, 0, screen->height * screen->width * sizeof(uint32_t));

    #pragma omp parallel for schedule(dynamic)// collapse(2)
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {
            scg::Ray ray = camera.getRay(x, y);

            scg::Vec3f colour = scg::trace(ray, objects, 1);

            PutPixelSDL(screen, x, y, scg::Vec3f(colour.r, colour.g, colour.b));
            /*
            scg::Intersection intersection;
            scg::Material material;

            if (scg::getClosestIntersection(ray, objects, intersection, material))
            {
                scg::Vec3f colour = material.getColour(intersection.uv);
                float light = std::max(scg::dot(intersection.normal, scg::normalise(-ray.direction)), 0.1f);
                colour *= light;

                PutPixelSDL(screen, x, y, scg::Vec3f(colour.r, colour.g, colour.b));
            }
            */
        }
    }
}

/*Place updates of parameters here*/
bool Update()
{
    static int t = SDL_GetTicks();
    /* Compute frame time */
    int t2 = SDL_GetTicks();
    float dt = float(t2 - t);
    t = t2;
    /*Good idea to remove this*/
    std::cout << "Render time: " << dt << " ms." << std::endl;

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        } else if (e.type == SDL_KEYDOWN)
        {
            int key_code = e.key.keysym.sym;
            switch (key_code)
            {
                case SDLK_ESCAPE:
                    /* Move camera quit */
                    return false;
                case SDLK_w:
                    /* Move camera forward */
                    camera.position.z += 0.2f;
                    break;
                case SDLK_s:
                    /* Move camera backwards */
                    camera.position.z -= 0.2f;
                    break;
                case SDLK_a:
                    /* Move camera left */
                    camera.position.x -= 0.2f;
                    break;
                case SDLK_d:
                    /* Move camera right */
                    camera.position.x += 0.2f;
                    break;
                case SDLK_UP:
                    /* Move rotate up */
                    camera.rotation.x += 5;
                    break;
                case SDLK_DOWN:
                    /* Move rotate down */
                    camera.rotation.x -= 5;
                    break;
                case SDLK_LEFT:
                    /* Move rotate left */
                    camera.rotation.y -= 5;
                    break;
                case SDLK_RIGHT:
                    /* Move rotate right */
                    camera.rotation.y += 5;
                    break;
            }
        }
    }
    return true;
}
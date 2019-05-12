#include "camera.h"
#include "pathtrace.h"
#include "ray.h"
#include "raytrace.h"
#include "sampler.h"
#include "scene.h"
#include "SDLauxiliary.h"
#include "utils.h"
#include "vector_type.h"

#include <SDL.h>

#include <iostream>
#include <cstdint>
#include <cstring>
#include <memory>
#include <omp.h>

#define RES 600
#define SCREEN_WIDTH  RES
#define SCREEN_HEIGHT RES

#define FULLSCREEN_MODE false

#undef main // SDL2 compatibility with Windows

// FUNCTIONS
bool Update();
void Draw(screen *screen);
void InitialiseBuffer();



scg::Sampler sampler[20]; // TODO: !!! find a better solution

scg::Camera camera{
    scg::Vec3f(0, 0, -3),
    scg::Vec3f(0, 0, 0),
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    true, // Jitter
    0.2f, // Aperture
    3.0f}; // Focal length

scg::Scene scene;

int samples;
scg::Vec3f buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

int main(int argc, char *argv[])
{
    InitialiseBuffer();

    screen *screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE);

    // Load scene
    scene = scg::LoadTestModel();

    // Start main loop
    while (Update())
    {
        Draw(screen);
        SDL_Renderframe(screen);
    }

    // Save and finish
    SDL_SaveImage(screen, "screenshot.bmp");
    KillSDL(screen);

    return 0;
}

void Draw(screen *screen)
{
    ++samples;

    // TODO: reseed generator
    #pragma omp parallel for schedule(dynamic) collapse(2)
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {
            scg::Ray ray = camera.getLensRay(x, y, sampler[omp_get_thread_num()]);
            ray.minT = scg::RAY_EPS;

            int depth = 10;
            float gamma = 3.0f;
            scg::Vec3f colour = scg::trace(scene, ray, depth, sampler[omp_get_thread_num()]);
            buffer[y][x] += colour * gamma; // TODO: clamp value

            PutPixelSDL(screen, x, y, buffer[y][x] / samples);
        }
    }
}

bool Update()
{
    static int t = SDL_GetTicks();
    /* Compute frame time */
    int t2 = SDL_GetTicks();
    float dt = float(t2 - t);
    t = t2;
    /*Good idea to remove this*/
    std::cout << "Iteration: " << samples << ". Render time: " << dt << " ms." << std::endl;

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
                    InitialiseBuffer();
                    break;
                case SDLK_s:
                    /* Move camera backwards */
                    camera.position.z -= 0.2f;
                    InitialiseBuffer();
                    break;
                case SDLK_a:
                    /* Move camera left */
                    camera.position.x -= 0.2f;
                    InitialiseBuffer();
                    break;
                case SDLK_d:
                    /* Move camera right */
                    camera.position.x += 0.2f;
                    InitialiseBuffer();
                    break;
                case SDLK_r:
                    InitialiseBuffer();
                    break;
                case SDLK_UP:
                    /* Move rotate up */
                    camera.rotation.x += 5;
                    InitialiseBuffer();
                    break;
                case SDLK_DOWN:
                    /* Move rotate down */
                    camera.rotation.x -= 5;
                    InitialiseBuffer();
                    break;
                case SDLK_LEFT:
                    /* Move rotate left */
                    camera.rotation.y -= 5;
                    InitialiseBuffer();
                    break;
                case SDLK_RIGHT:
                    /* Move rotate right */
                    camera.rotation.y += 5;
                    InitialiseBuffer();
                    break;
            }
        }
    }
    return true;
}

void InitialiseBuffer()
{
    samples = 0;
    memset(buffer, 0, sizeof(buffer));
}
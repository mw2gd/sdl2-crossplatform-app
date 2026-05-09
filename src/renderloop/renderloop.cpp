#include <iostream>
#include <algorithm>
#include <cmath>
#include <SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "renderloop.hpp"

#ifdef __EMSCRIPTEN__
EM_JS(int, canvas_get_width, (), {
    return Module.canvas.clientWidth;
});

EM_JS(int, canvas_get_height, (), {
    return Module.canvas.clientHeight;
});

#endif

namespace
{
constexpr float Pi = 3.14159265359f;
constexpr int WindowWidth = 640;
constexpr int WindowHeight = 320;

Uint8 colorChannel(float time, float offset)
{
    return static_cast<Uint8>(127.0f + 128.0f * std::sin(time + offset));
}
}

void Render::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            requestQuit();
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        {
            requestQuit();
        }
    }
}

void Render::drawCircle(int centerX, int centerY, int radius)
{
    constexpr int segments = 96;

    for (int index = 0; index < segments; ++index)
    {
        float angleA = (static_cast<float>(index) / segments) * Pi * 2.0f;
        float angleB = (static_cast<float>(index + 1) / segments) * Pi * 2.0f;

        int xA = centerX + static_cast<int>(std::cos(angleA) * radius);
        int yA = centerY + static_cast<int>(std::sin(angleA) * radius);
        int xB = centerX + static_cast<int>(std::cos(angleB) * radius);
        int yB = centerY + static_cast<int>(std::sin(angleB) * radius);

        SDL_RenderDrawLine(renderer, xA, yA, xB, yB);
    }
}

void Render::drawScene(int width, int height, float time)
{
    int centerX = width / 2;
    int centerY = height / 2;

    SDL_SetRenderDrawColor(renderer, 8, 10, 18, 255);
    SDL_RenderClear(renderer);

    drawRadialLines(centerX, centerY, time);
    drawPulsingRings(centerX, centerY, time);
    drawOrbiters(centerX, centerY, width, height, time);
}

void Render::drawRadialLines(int centerX, int centerY, float time)
{
    // A rotating fan of colored lines gives the scene most of its motion.
    for (int index = 0; index < 42; ++index)
    {
        float phase = time * 1.4f + index * 0.32f;
        float radius = 30.0f + index * 7.0f;
        int x = centerX + static_cast<int>(std::cos(phase) * radius);
        int y = centerY + static_cast<int>(std::sin(phase * 1.3f) * radius * 0.55f);

        SDL_SetRenderDrawColor(
            renderer,
            colorChannel(time * 2.0f, index * 0.25f),
            colorChannel(time * 1.7f, index * 0.35f + 2.0f),
            colorChannel(time * 1.3f, index * 0.45f + 4.0f),
            255);

        SDL_RenderDrawLine(renderer, centerX, centerY, x, y);
    }
}

void Render::drawPulsingRings(int centerX, int centerY, float time)
{
    // SDL2 has line primitives but no built-in circle primitive, so circles
    // are approximated with short connected line segments.
    for (int index = 0; index < 7; ++index)
    {
        float pulse = std::sin(time * 2.0f + index * 0.8f);
        int radius = 28 + index * 24 + static_cast<int>(pulse * 8.0f);

        SDL_SetRenderDrawColor(
            renderer,
            colorChannel(time * 1.6f, index * 0.7f + 1.0f),
            colorChannel(time * 1.2f, index * 0.6f + 3.0f),
            colorChannel(time * 1.8f, index * 0.5f + 5.0f),
            255);

        drawCircle(centerX, centerY, radius);
    }
}

void Render::drawOrbiters(int centerX, int centerY, int width, int height, float time)
{
    int orbitRadius = std::min(width, height) / 4;
    for (int index = 0; index < 10; ++index)
    {
        float angle = time * 1.8f + index * Pi * 0.2f;
        int x = centerX + static_cast<int>(std::cos(angle) * orbitRadius);
        int y = centerY + static_cast<int>(std::sin(angle) * orbitRadius);
        int size = 8 + static_cast<int>(std::sin(time * 3.0f + index) * 4.0f);

        SDL_Rect rect = {x - size, y - size, size * 2, size * 2};
        SDL_SetRenderDrawColor(renderer, 255, 245, 180, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

bool Render::render()
{
    if (!renderer)
    {
        std::cout << "Cannot render before initialization." << std::endl;
        return false;
    }

    handleEvents();

    int width = 0;
    int height = 0;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    float time = SDL_GetTicks() / 1000.0f;
    drawScene(width, height, time);

    SDL_RenderPresent(renderer);

#ifdef __EMSCRIPTEN__
    // Match SDL's window size to the CSS-sized browser canvas each frame.
    SDL_SetWindowSize(window, canvas_get_width(), canvas_get_height());
#else
    SDL_Delay(10);
#endif

    return true;
}

bool Render::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;
    SDL_CreateWindowAndRenderer(WindowWidth, WindowHeight, flags, &window, &renderer);
    if (!window || !renderer)
    {
        std::cout << "Could not create window + renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Render::kill()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

bool Render::shouldQuit() const
{
    return quit;
}

void Render::requestQuit()
{
    quit = true;
}

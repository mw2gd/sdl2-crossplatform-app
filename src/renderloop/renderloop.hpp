#pragma once

#include <SDL.h>

class Render
{
public:
    bool init();
    void kill();
    bool render();

    bool shouldQuit() const;
    void requestQuit();

private:
    void handleEvents();
    void drawCircle(int centerX, int centerY, int radius);
    void drawScene(int width, int height, float time);
    void drawRadialLines(int centerX, int centerY, float time);
    void drawPulsingRings(int centerX, int centerY, float time);
    void drawOrbiters(int centerX, int centerY, int width, int height, float time);

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool quit = false;
};

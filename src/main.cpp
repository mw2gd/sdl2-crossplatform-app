#include <iostream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "renderloop.hpp"

namespace
{
Render render;
}

void mainloop(void)
{
    if (!render.render())
    {
        std::cout << "Error rendering: " << std::endl;
        render.requestQuit();
    }

#ifdef __EMSCRIPTEN__
    if (render.shouldQuit())
    {
        render.kill();
        emscripten_cancel_main_loop();
    }
#endif
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (!render.init())
    {
        return 1;
    }

#ifdef __EMSCRIPTEN__
    // In the browser, Emscripten owns the event loop and calls mainloop.
    emscripten_set_main_loop(mainloop, 60, 1);
#else
    while (!render.shouldQuit())
    {
        mainloop();
    }
#endif

    render.kill();

    return 0;
}

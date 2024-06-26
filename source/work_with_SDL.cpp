#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "work_with_SDL.h"
#include "html_logfile.h"

#undef  LOG
#define LOG(...) 


int initializeSDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        return ErrorsSDL::NO_SDL_INIT;
    }

    *window = SDL_CreateWindow("Mandelbrot Set", 
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                               width, height, SDL_WINDOW_SHOWN);
    if (!*window)
    {
        printf("Couldn't create window: %s\n", SDL_GetError());
        return ErrorsSDL::NO_WINDOW;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Couldn't create renderer: %s\n", SDL_GetError());
        return ErrorsSDL::NO_RENDERER;
    }

    return EXIT_SUCCESS;
}

int quitSDL(SDL_Window **window, SDL_Renderer **renderer)
{
    assert(window);
    assert(renderer);
    assert(*window);
    assert(*renderer);

    SDL_DestroyRenderer(*renderer);
    *renderer = NULL;

    SDL_DestroyWindow(*window);
    *window = NULL;

    SDL_Quit();

    return EXIT_SUCCESS;
}

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "work_with_SDL.h"

const int MaxPointN = 256;


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

    SDL_DestroyWindow(*window);
    *window = NULL;

    SDL_DestroyRenderer(*renderer);
    *renderer = NULL;

    SDL_Quit();

    return EXIT_SUCCESS;
}


#define BYTE char

void printPoint(SDL_Renderer *renderer,
               int x, int y, int N)
{
    float I = sqrtf((float)N / (float)MaxPointN) * 255.f;
    
    BYTE c = (BYTE) I;

    if (N >= MaxPointN) SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    else                SDL_SetRenderDrawColor(renderer, 
                                               (BYTE)(255 - c), (BYTE)(c%2 * 64),
                                                c , SDL_ALPHA_OPAQUE);

    SDL_RenderDrawPoint(renderer, x, y);
}

#undef BYTE

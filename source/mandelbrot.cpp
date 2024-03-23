#include <stdio.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "work_with_SDL.h"
#include "mandelbrot.h"

const int WindowWidth  = 800;
const int WindowHeight = 600;

const int MaxPointN    = 256;

const float dx         = 1.f / WindowWidth;
const float dy         = 1.f / WindowHeight;

const float Radius2Max = 100.f;

const float CenterX    = -1.325f;
const float CenterY    = 0.f;



static int printSetToWindow(SDL_Window *window, SDL_Renderer *renderer, 
                            int width, int height);

static int calculatePointN(float x0, float y0);


int printMandelbrotSet()
{
    SDL_Window   *window   = NULL;
    SDL_Renderer *renderer = NULL;

    int initError = initializeSDL(&window, &renderer, WindowWidth, WindowHeight);
    if (initError)  return initError;

    printSetToWindow(window, renderer, WindowWidth, WindowHeight);

    quitSDL(&window, &renderer);

    return EXIT_SUCCESS;
}

static int printSetToWindow(SDL_Window *window, SDL_Renderer *renderer, 
                            int width, int height)
{
    assert(window);
    assert(renderer);

    float shiftX = 0,
          shiftY = 0;

    float zoomMult     = 1.f;

    SDL_Event event = {};

    while (true)
    {
        SDL_PollEvent(&event);

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE) break;
        }

        SDL_RenderClear(renderer);

        for (int curY = 0; curY < height; curY++)
        {
            float dxZoomed = dx * zoomMult;

            float x0 = ((             - WindowWidth / 2.f) * dx + CenterX + shiftX) * zoomMult;
            float y0 = (( (float)curY - WindowWidth / 2.f) * dy + CenterY + shiftY) * zoomMult;

            for (int curX = 0; curX < width; curX++, x0 += dx * dxZoomed)
            {
                int N = calculatePointN(x0, y0);

                printPoint(renderer, curX, curY, N);
            }
        }

        SDL_RenderPresent(renderer);
    }

    return EXIT_SUCCESS;
}

static int calculatePointN(float x0, float y0)
{
    float x = x0, 
    y = y0;

    int N = 0;
    for ( ; N < MaxPointN; N++)
    {
        float x2 = x * x,
              y2 = y * y,
              xy = x * y;

        float r2 = x2 + y2;

        if (r2 >= Radius2Max) return N;

        x = x2 - y2 + x0;
        y = xy + xy + y0;
    }

    return MaxPointN;
}

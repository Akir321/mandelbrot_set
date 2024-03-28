#include <SDL2/SDL.h>
#include <assert.h>

#include "mandelbrot.h"


void calculateSet(SDL_Renderer *renderer, const CoordData *data);

static inline int  calculatePointN(float x0, float y0);

static inline void printPoint(SDL_Renderer *renderer, 
                              int x, int y, int N);


void calculateSet(SDL_Renderer *renderer, const CoordData *data)
{
    assert(renderer);
    assert(data);

    float dxZoomed = dx * data->zoomMult;

    for (int curY = 0; curY < WindowHeight; curY++)
    {
        float x0 = ( (            - WindowWidth  / 2.f) * dx ) * data->zoomMult +
                    CenterX + data->shiftX;

        float y0 = ( ((float)curY - WindowHeight / 2.f) * dx ) * data->zoomMult +
                    CenterY + data->shiftY;

        for (int curX = 0; curX < WindowWidth; curX++, x0 += dxZoomed)
        {
            int N = calculatePointN(x0, y0);

            printPoint(renderer, curX, curY, N);
        }
    }
}

static inline int calculatePointN(float x0, float y0)
{
    int N = 0;

#ifndef BASELINE

    float x = x0, 
          y = y0;

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

#endif

    return N;
}

#define BYTE char

static inline void printPoint(SDL_Renderer *renderer, 
                              int x, int y, int N)
{
    float I = sqrtf((float)N / (float)MaxPointN) * 255.f;
    
    BYTE c = (BYTE) I;

    if (N >= MaxPointN) 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    else                
        SDL_SetRenderDrawColor(renderer, 0, (BYTE)(c%2 * 64), (BYTE)(c%2 * 64),
                               SDL_ALPHA_OPAQUE);

    SDL_RenderDrawPoint(renderer, x, y);
}

#undef BYTE

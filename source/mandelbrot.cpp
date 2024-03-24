#include <stdio.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "work_with_SDL.h"
#include "mandelbrot.h"
#include "html_logfile.h"

const int WindowWidth  = 800;
const int WindowHeight = 600;

const int MaxPointN    = 256;

const float dx         = 1.f / WindowWidth;
const float dy         = 1.f / WindowHeight;

const float Radius2Max = 100.f;

const float CenterX    =  -1.325f;
const float CenterY    =  0.f;

#undef  LOG
#define LOG(...) 



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

    float zoomMult  = 1.f;

    bool repeat     = true;
    SDL_Event event = {};

    while (repeat)
    {
        SDL_PollEvent(&event);

        if (event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:   repeat = false;
                                    break;

                case SDLK_UP:       shiftY -= dy * 10;
                                    break;

                case SDLK_DOWN:     shiftY += dy * 10;
                                    break;

                case SDLK_RIGHT:    shiftX += dx * 10;
                                    break;

                case SDLK_LEFT:     shiftX -= dx * 10;
                                    break;
                                
                case SDLK_z:        zoomMult += dx * 10;
                                    break;

                case SDLK_o:        zoomMult -= dx * 10;
                                    break;

                default:            break;
            }
        }

        SDL_RenderClear(renderer);

        for (int curY = 0; curY < height; curY++)
        {
            float dxZoomed = dx * zoomMult;

            float x0 = ( (            - WindowWidth  / 2.f) * dx + shiftX ) * zoomMult + CenterX;
            float y0 = ( ((float)curY - WindowHeight / 2.f) * dy + shiftY ) * zoomMult + CenterY;

            for (int curX = 0; curX < width; curX++, x0 += dxZoomed)
            {
                LOG("curX = %d, curY = %d\n", curX, curY);
                LOG("x0 = %f, y0 = %f\n", x0, y0);
                int N = calculatePointN(x0, y0);
                LOG("N = %d\n", N);

                printPoint(renderer, curX, curY, N);
            }
        }

        SDL_RenderPresent(renderer);
        //repeat = false;
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
        LOG("x = %f, y = %f\n", x, y);
        float x2 = x * x,
              y2 = y * y,
              xy = x * y;

        float r2 = x2 + y2;
        LOG("x^2 = %f, y^2 = %f, r^2 = %f\n", x2, y2, r2);

        if (r2 >= Radius2Max) return N;

        x = x2 - y2 + x0;
        y = xy + xy + y0;
    }

    return MaxPointN;
}

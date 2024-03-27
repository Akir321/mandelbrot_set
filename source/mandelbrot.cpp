#include <stdio.h>
#include <assert.h>
#include <immintrin.h>
#include <SDL2/SDL.h>

#include "work_with_SDL.h"
#include "mandelbrot.h"
#include "html_logfile.h"


const float changeMult = 5.f;

const float dChangeX   = dx * changeMult;
const float dChangeY   = dy * changeMult;

static void pollAllSDLEvents(bool *setIsOn, CoordData *data);

static void printSetToWindow(SDL_Window *window, SDL_Renderer *renderer);



extern void calculateSet(SDL_Renderer *renderer, const CoordData *data);


int runMandelbrotSet()
{
    SDL_Window   *window   = NULL;
    SDL_Renderer *renderer = NULL;

    int initError = initializeSDL(&window, &renderer, WindowWidth, WindowHeight);
    if (initError)  return initError;

    printSetToWindow(window, renderer);

    quitSDL(&window, &renderer);

    return EXIT_SUCCESS;
}

static void printSetToWindow(SDL_Window *window, SDL_Renderer *renderer)
{
    assert(window);
    assert(renderer);

    CoordData data = {}; 

    data.shiftX    = 0;
    data.shiftY    = 0;
    data.zoomMult  = 1.f;

    bool setIsOn    = true;

    while (setIsOn)
    {
        pollAllSDLEvents(&setIsOn, &data);

        SDL_RenderClear(renderer);

        calculateSet(renderer, &data);

        SDL_RenderPresent(renderer);
    }
}

static void pollAllSDLEvents(bool *setIsOn, CoordData *data)
{
    assert(data);

    SDL_Event event = {};

    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:   *setIsOn        = false;    break;

                case SDLK_UP:       data->shiftY   -= dChangeY; break;

                case SDLK_DOWN:     data->shiftY   += dChangeY; break;

                case SDLK_RIGHT:    data->shiftX   += dChangeX; break;

                case SDLK_LEFT:     data->shiftX   -= dChangeX; break;
                                    
                case SDLK_z:        if (data->zoomMult -  dChangeX > 0) 
                                        data->zoomMult -= dChangeX;
                                                                break;

                case SDLK_x:        data->zoomMult += dChangeX; break;

                case SDLK_r:        data->zoomMult = 1.f; 
                                    data->shiftX = 0, data->shiftY = 0;
                                                                break;

                default:                                        break;
            }
        }
    }
}


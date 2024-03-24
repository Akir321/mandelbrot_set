#include <stdio.h>
#include <assert.h>
#include <immintrin.h>
#include <SDL2/SDL.h>

#include "work_with_SDL.h"
#include "mandelbrot.h"
#include "html_logfile.h"

const int WindowWidth  = 800;
const int WindowHeight = 600;

const int MaxPointN    = 256;

const int NumsInVector = 8;

const float dx         = 1.f / WindowWidth;
const float dy         = 1.f / WindowHeight;

const float Radius2Max = 100.f;

const float CenterX    =  -1.325f;
const float CenterY    =  0.f;

const float changeMult = 10.f;

const __m256 NMaxes = _mm256_set1_ps(MaxPointN);
const __m256 r2Max  = _mm256_set1_ps(Radius2Max);
const __m256 _255   = _mm256_set1_ps(255.f);

#undef  LOG
#define LOG(...) 


static void calculateSet(SDL_Renderer *renderer, const CoordData *data);

static void printSetToWindow(SDL_Window *window, SDL_Renderer *renderer);

static inline __m256i calculateNOf8Points(__m256 X0,  __m256 Y0);

static inline void    print8Points(SDL_Renderer *renderer,
                                   int x, int y, __m256i N);


int printMandelbrotSet()
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

    bool repeat     = true;
    SDL_Event event = {};

    while (repeat)
    {
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:   repeat         = false;           break;

                    case SDLK_UP:       data.shiftY   -= dy * changeMult; break;

                    case SDLK_DOWN:     data.shiftY   += dy * changeMult; break;

                    case SDLK_RIGHT:    data.shiftX   += dx * changeMult; break;

                    case SDLK_LEFT:     data.shiftX   -= dx * changeMult; break;
                                    
                    case SDLK_z:        data.zoomMult -= dx * changeMult; break;

                    case SDLK_o:        data.zoomMult += dx * changeMult; break;

                    default:                                              break;
                }
            }
        }

        SDL_RenderClear(renderer);

        calculateSet(renderer, &data);

        SDL_RenderPresent(renderer);
    }
}

static void calculateSet(SDL_Renderer *renderer, const CoordData *data)
{
    const __m256 _76543210 = _mm256_set_ps (7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);

    for (int curY = 0; curY < WindowHeight; curY++)
    {
        float NumDxZoomed = NumsInVector * dx * data->zoomMult;

        float x0 = ( (            - WindowWidth  / 2.f) * dx + data->shiftX ) * 
                        data->zoomMult + CenterX;

        float y0 = ( ((float)curY - WindowHeight / 2.f) * dy + data->shiftY ) * 
                        data->zoomMult + CenterY;


        for (int curX = 0; curX < WindowWidth; curX += NumsInVector, x0 += NumDxZoomed)
        {
            __m256 X0 = _mm256_fmadd_ps(_mm256_set1_ps(x0), _76543210,
                                        _mm256_set1_ps(dx));

            __m256 Y0 = _mm256_set1_ps(y0);

            __m256i N = calculateNOf8Points(X0, Y0);

            print8Points(renderer, curX, curY, N);
        }
    }
}

static inline __m256i calculateNOf8Points(__m256 X0,  __m256 Y0)
{
    __m256 X = X0;
    __m256 Y = Y0;

    __m256i N = _mm256_setzero_si256();

    for (int n = 0; n < MaxPointN; n++)
    {
        __m256 x2 = _mm256_mul_ps(X, X);
        __m256 y2 = _mm256_mul_ps(Y, Y);

        __m256 r2 = _mm256_add_ps(x2, y2);

        __m256 cmp = _mm256_cmp_ps(r2, r2Max, _CMP_NGT_UQ);
        int mask   = _mm256_movemask_ps(cmp);
        if (!mask) break;

        N = _mm256_sub_epi32(N, _mm256_castps_si256(cmp));

        __m256 xy = _mm256_mul_ps(X, Y);
        
        X = _mm256_add_ps(_mm256_sub_ps(x2, y2), X0);
        Y = _mm256_add_ps(_mm256_add_ps(xy, xy), Y0);
    }

    return N;
}


#define BYTE char

static inline void print8Points(SDL_Renderer *renderer,
                                int x, int y, __m256i N)
{
    __m256 I =  _mm256_mul_ps(
                              _mm256_sqrt_ps(_mm256_sqrt_ps( 
                              _mm256_div_ps(_mm256_cvtepi32_ps(N), NMaxes))) , _255);
    
    float *ptrI = (float *) &I;
    int   *ptrN = (int *)   &N;

    for (int i = 0; i < NumsInVector; i++)
    {
        BYTE c = (BYTE) ptrI[i];

        if (ptrN[i] >= MaxPointN) 
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 
                                   SDL_ALPHA_OPAQUE);
        else                      
            SDL_SetRenderDrawColor(renderer, (BYTE)(255 - c), (BYTE)(c%2 * 64), c,
                                   SDL_ALPHA_OPAQUE);

        SDL_RenderDrawPoint(renderer, x + i, y);
    }
}

#undef BYTE

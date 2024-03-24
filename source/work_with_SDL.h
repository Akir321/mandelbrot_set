#ifndef  __WORK_WITH_SDL_H__
#define  __WORK_WITH_SDL_H__


enum ErrorsSDL
{
    NO_SDL_INIT = 1,
    NO_WINDOW   = 2,
    NO_RENDERER = 3,
};


int initializeSDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
int quitSDL      (SDL_Window **window, SDL_Renderer **renderer);


#endif //__WORK_WITH_SDL_H__

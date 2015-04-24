#include "turtle_header.h"


/*----------------------------------------------------------------------------------------------*/
/*-----------------------------INITIALIZE THE GRAPHICS AND SDL----------------------------------*/
/*----------------------------------------------------------------------------------------------*/

void SDLFunction (Graphics *graphics) {
    SDL_Init (SDL_INIT_EVERYTHING);
    graphicsInit(graphics);
}


void graphicsInit(Graphics *graphics)
{
    graphics->window = SDL_CreateWindow("Turtle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH,SCREENHEIGHT,0);
    graphics->renderer = SDL_CreateRenderer(graphics->window,-1,0);
    SDL_SetRenderDrawColor(graphics->renderer,0,0,0,255);
    SDL_RenderFillRect(graphics->renderer, NULL);
    SDL_SetRenderDrawColor(graphics->renderer,255,255,255,255);
}


/*----------------------------------------------------------------------------------------*/
/*---------------------TARCK THE EVENTS (KEY PRESSING) FROM KEYBOARD----------------------*/
/*----------------------------------------------------------------------------------------*/
void SDLEvents (Graphics *graphics)
{
        while (SDL_PollEvent(&graphics->event)) {

      switch (graphics->event.type) {

          case SDL_QUIT:
          SDL_Quit();
          break;

          case SDL_KEYDOWN:
            switch (graphics->event.key.keysym.sym) {

                case SDLK_ESCAPE:
                SDL_Quit();
                break;
              }
          break;
        }
      }
}



/*----------------------------------------------------------------------------------------*/
/*-----------------------------EXECUTES DRAWING ON THE SCREEN-----------------------------*/
/*----------------------------------------------------------------------------------------*/
void draw(Graphics *graphics)
{
    SDL_UpdateWindowSurface(graphics->window);
    SDL_RenderPresent(graphics->renderer);
    SDLEvents(graphics);
}

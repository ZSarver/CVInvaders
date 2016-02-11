#include <stdio.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>
#include "invader.h"

//int main(int nArg, char* args)
int main(int argc, char *argv[])
{

  //initialization
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
    {
      printf("oops you bwoke it \n SDL_Init failed \n");
      return 1;
    }
  if (SDL_VideoInit(NULL) != 0)
    {
      printf("oops you bwoke it \n SDL_VideoInit failed \n");
      return 1;
    }
  if (TTF_Init() != 0)
    {
      printf("oops you bwoke it \n TTF_Init failed \n");
      return 1;
    }

  //create window
  SDL_Window* window  = SDL_CreateWindow("test", 100, 100, 800, 600, 0);

  //set up renderer
  SDL_Renderer* rend  = SDL_CreateRenderer(window, -1,
                                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  //open font
  TTF_Font* orbitron = TTF_OpenFont("theleagueof-orbitron-13e6a52\\Orbitron Light.ttf",15);
  if (!orbitron)
    {
      printf("oops you bwoke it \n TTF_OpenFont failed \n");
      return 1;
    }
  SDL_SetTextureBlendMode(textTex,SDL_BLENDMODE_BLEND);

  //make a few invaders
  Invader* inv1 = createInvader('Z', 100, 100, rend, orbitron);
  Invader* inv2 = createInvader('a', 120, 100, rend, orbitron);
  Invader* inv3 = createInvader('c', 140, 100, rend, orbitron);
  
  //dark grey, cooler than black
  SDL_SetRenderDrawColor(rend, 25, 25, 25, 255);
  //event loop
  SDL_Event event;
  bool shouldQuit = false;
  double frame = 0.0;
  while (!shouldQuit)
    {
      //let's draw some stuff
      SDL_RenderClear(rend);
      SDL_RenderCopy(rend, inv1->tex, NULL, inv1->screenPos);
      SDL_RenderCopy(rend, inv2->tex, NULL, inv2->screenPos);
      SDL_RenderCopy(rend, inv3->tex, NULL, inv3->screenPos);
      SDL_RenderPresent(rend);
      
      //get all the events this frame
      while (SDL_PollEvent(&event))
        {
          if (event.type == SDL_QUIT)
            {
              shouldQuit = true;
            }
        }
      frame = frame + 0.5;
    }

  destroyInvader(inv1);
  destroyInvader(inv2);
  destroyInvader(inv3);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
  return 0;
}

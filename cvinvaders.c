#include <stdio.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>

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

  //create window
  SDL_Window* window  = SDL_CreateWindow("test", 100, 100, 800, 600, 0);

  //set up renderer
  SDL_Renderer* rend  = SDL_CreateRenderer(window, -1,
                                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  /* testing only */
  //set up texture
  SDL_Surface* surf = SDL_LoadBMP("test.bmp");
  SDL_Texture*  tex = SDL_CreateTextureFromSurface(rend, surf);
  SDL_FreeSurface(surf);
  SDL_Rect rect;
  rect.x = 350;
  rect.y = 250;
  rect.h = 200;
  rect.w = 200;
  

  //dark grey, cooler than black
  SDL_SetRenderDrawColor(rend, 130, 130, 130, 255);
  //event loop
  SDL_Event event;
  bool shouldQuit = false;
  double frame = 0.0;
  while (!shouldQuit)
    {
      //let's draw some stuff
      SDL_RenderClear(rend);
      SDL_RenderCopyEx(rend, tex, NULL, &rect, frame, NULL, SDL_FLIP_NONE);
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
  
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

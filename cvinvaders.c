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
  
  //open font
  TTF_Font* orbitron = TTF_OpenFont("theleagueof-orbitron-13e6a52\\Orbitron Light.ttf",15);
  if (!orbitron)
    {
      printf("oops you bwoke it \n TTF_OpenFont failed \n");
      return 1;
    }
  SDL_Color textColor;
  textColor.r = 255;
  textColor.g = 255;
  textColor.b = 255;
  textColor.a = 255;
  SDL_Rect textRect;
  textRect.x = 50;
  textRect.y = 50;
  textRect.h = 15;
  textRect.w = 200;
  SDL_Surface* text = TTF_RenderText_Blended(orbitron, "oh hello there", textColor);
  if (text == NULL)
    {
      printf("oops you bwoke it \n TTF_RenderText_Blended failed \n");
      return 1;
    }
  SDL_Texture* textTex = SDL_CreateTextureFromSurface(rend, text);
  if (textTex == NULL)
    {
      printf("oops you bwoke it \n SDL_CreateTextureFromSurface failed \n");
      return 1;
    }
  SDL_SetTextureBlendMode(textTex,SDL_BLENDMODE_BLEND);
  
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
      SDL_RenderCopy(rend, textTex, NULL, &textRect);
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
  TTF_Quit();
  SDL_Quit();
  return 0;
}

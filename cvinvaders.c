#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "invader.h"
#include "ship.h"

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
      return 2;
    }
  if (TTF_Init() != 0)
    {
      printf("oops you bwoke it \n TTF_Init failed \n");
      return 3;
    }

  //create window
  SDL_Window* window  = SDL_CreateWindow("test", 100, 100, 800, 600, 0);

  //set up renderer
  SDL_Renderer* rend  =
    SDL_CreateRenderer(window, -1,
                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  //open font
  TTF_Font* orbitron =
    TTF_OpenFont("theleagueof-orbitron-13e6a52\\Orbitron Light.otf",48);
  if (!orbitron)
    {
      printf("oops you bwoke it \n TTF_OpenFont failed \n");
      return 1;
    }
  TTF_SetFontHinting(orbitron, TTF_HINTING_MONO);

  //invader wave time!
  Invader** wave = (Invader**)malloc(sizeof(Invader*)*55);
  SDL_RWops* text = SDL_RWFromFile("cvtext.txt", "r");
  if (loadInvaderWave(text, wave, rend, orbitron) != 0) {
    printf("failed to load invader wave \n");
    return 4;
  }

  //let's make a ship and a bullet
  Ship* ship = createShip(400, 500, rend, orbitron);
  Bullet* bullet = createBullet(400, 400, rend, orbitron);
  
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
      for (int i = 0; i < 55; i++) {
        if (wave[i] != NULL) {
          SDL_RenderCopy(rend, wave[i]->tex, NULL, wave[i]->hitbox);
        }
      }
      SDL_RenderCopyEx(rend, ship->tex, NULL, ship->hitbox, 0.0,
                       NULL, SDL_FLIP_VERTICAL);
      SDL_RenderCopy(rend, bullet->tex, NULL, bullet->hitbox);
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

  /* cleanup on aisle here */
  for (int i = 0; i < 55; i++) {
    if (wave[i] != NULL) {
      destroyInvader(wave[i]);
    }
  }
  destroyShip(ship);
  destroyBullet(bullet);
  SDL_FreeRW(text);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
  return 0;
}

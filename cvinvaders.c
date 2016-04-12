#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "invader.h"
#include "ship.h"
#include "common.h"

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

  //let's make a ship
  Ship* ship = createShip(400, 500, rend, orbitron);

  //points!
  int score = 0;
  int oldscore = -1;
  SDL_Surface* scorelabelsurf = NULL;
  SDL_Texture* scorelabeltex = NULL;
  SDL_Surface* scoresurf = NULL;
  SDL_Texture* scoretex = NULL;
  char scorestring[20] = "";
  itoa(score*100, scorestring, 10);
  SDL_Rect scorelabelbox;
  scorelabelbox.x = 0;
  scorelabelbox.y = 0;
  scorelabelbox.h = WIDTH/2;
  scorelabelbox.w = 50;
  SDL_Rect scorebox;
  scorebox.x = 51;
  scorebox.y = 0;
  scorebox.h = WIDTH/2;
  scorebox.w = WIDTH/2;
  SDL_Color scorecolor;
  scorecolor.r = 255;
  scorecolor.g = 255;
  scorecolor.b = 255;
  scorecolor.a = 255;
  
  scorelabelsurf = TTF_RenderText_Blended(orbitron, "Score: ", scorecolor);
  if (scorelabelsurf == NULL) {
    printf("Error creating score surface! %s\n", SDL_GetError());
    exit(1);
  }
  scorelabeltex = SDL_CreateTextureFromSurface(rend,scorelabelsurf);
  if (scorelabeltex == NULL){
    printf("Error creatings score texture! %s\n", SDL_GetError());
    exit(1);
  }
  
  //dark grey, cooler than black
  SDL_SetRenderDrawColor(rend, 25, 25, 25, 255);
  //event loop
  SDL_Event event;
  bool shouldQuit = false;
  unsigned int oldTime = 0;
  unsigned int curTime = 0;
  while (!shouldQuit){
    //update the timer
    curTime = SDL_GetTicks() - oldTime;
    oldTime = SDL_GetTicks();
    //let's draw some stuff
    SDL_RenderClear(rend);
    for (int i = 0; i < WAVELENGTH; i++) {
      if (wave[i] != NULL) {
        SDL_RenderCopy(rend, wave[i]->tex, NULL, wave[i]->hitbox);
      }
    }
    SDL_RenderCopyEx(rend, ship->tex, NULL, ship->hitbox, 0.0,
                     NULL, SDL_FLIP_VERTICAL);
    if (ship->bullet != NULL) {
      SDL_RenderCopy(rend, ship->bullet->tex, NULL, ship->bullet->hitbox);
    }

    //render score. It can only change like once every 300 frames so might as
    //well render it nice
    if (score != oldscore){
      itoa(score*100, scorestring, 10);
      scoresurf = TTF_RenderText_Blended(orbitron, scorestring, scorecolor);
      if (scoresurf == NULL) {
        printf("Error creating score surface! %s\n", SDL_GetError());
        exit(1);
      }
      scoretex = SDL_CreateTextureFromSurface(rend,scoresurf);
      if (scoretex == NULL){
        printf("Error creatings score texture! %s\n", SDL_GetError());
        exit(1);
      }
      oldscore = score;
      scorebox.w = WIDTH/2 * (int)log10((score)*1000);
    }
    SDL_RenderCopy(rend, scorelabeltex, NULL, &scorelabelbox);
    SDL_RenderCopy(rend, scoretex, NULL, &scorebox);
    
    SDL_RenderPresent(rend);
    
    //get all the events this frame
    while (SDL_PollEvent(&event)){
      switch (event.type) {
      case SDL_QUIT:
        shouldQuit = true;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
          if (ship->hitbox->x > WIDTH) {
            ship->vel = -1;
          }
          break;
        case SDLK_RIGHT:
          if (ship->hitbox->x < 800 - WIDTH) {
            ship->vel = 1;
          }
          break;
        case SDLK_UP:
        case SDLK_SPACE:
          if (ship->bullet == NULL) {
            ship->bullet = createBullet(ship->hitbox->x, ship->hitbox->y + 10,
                                        rend, orbitron);
          }
          break;
        case SDLK_ESCAPE:
          shouldQuit = true;
          break;
        default:
          break;
        }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
        case SDLK_RIGHT:
          ship->vel = 0;
          break;
        default:
          break;
        }
      }
    }
    //collision detection
    for (int i = 0; i < WAVELENGTH; i++) {
      if(wave[i] != NULL && ship->bullet != NULL) {
        if(SDL_HasIntersection(ship->bullet->hitbox, wave[i]->hitbox)) {
          score += 1;
          destroyInvader(wave[i]);
          destroyBullet(ship->bullet);
          wave[i] = NULL;
          ship->bullet = NULL;
        }
      }
    }
    
    //update objects
    if (ship->bullet != NULL) {
      ship->bullet->hitbox->y += ship->bullet->vel * SPEED * curTime;
      if (ship->bullet->hitbox->y < 0) {
        destroyBullet(ship->bullet);
        ship->bullet = NULL;
      }
    }
    if ((ship->hitbox->x >= WIDTH && ship->vel < 0)
        || (ship->hitbox->x <= 800 - 2 * WIDTH && ship->vel > 0)) {
      ship->hitbox->x += ship->vel * SPEED * curTime;
    }
  }
  /* cleanup on aisle here */
  for (int i = 0; i < 55; i++) {
    if (wave[i] != NULL) {
      destroyInvader(wave[i]);
    }
  }
  destroyShip(ship);
  SDL_FreeRW(text);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
  return 0;
}

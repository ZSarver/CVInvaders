#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "invader.h"
#include "ship.h"
#include "common.h"
#include "wave.h"
#include "cvinvaders.h"

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
  SDL_RWops* text = SDL_RWFromFile("cvtext.txt", "r");
  Wave *wave = (Wave*)malloc(sizeof(Wave));
  if (loadInvaderWave(text, wave, rend, orbitron) != 0) {
    printf("Error loading invader wave.");
    return 1;
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
  SDL_Color white;
  white.r = 255;
  white.g = 255;
  white.b = 255;
  white.a = 255;
  
  scorelabelsurf = TTF_RenderText_Blended(orbitron, "Score: ", white);
  if (scorelabelsurf == NULL) {
    printf("Error creating score surface! %s\n", SDL_GetError());
    exit(1);
  }
  scorelabeltex = SDL_CreateTextureFromSurface(rend,scorelabelsurf);
  if (scorelabeltex == NULL){
    printf("Error creatings score texture! %s\n", SDL_GetError());
    exit(1);
  }

  //set up instructions
  char *instructions[4];
  instructions[0] = "Arrow Keys to move!";
  instructions[1] = "Space Bar or Up to shoot!";
  instructions[2] = "Escape or close to quit!";
  instructions[3] = "Press any key to start!";
  SDL_Texture *instructionsTex[4];
  SDL_Surface *instructionsSurf;
  SDL_Rect instructionsBox[4];
  for (int i = 0; i < 4; i++) {
    instructionsSurf = TTF_RenderText_Blended(orbitron, instructions[i], white);
    instructionsTex[i] = SDL_CreateTextureFromSurface(rend, instructionsSurf);
    SDL_FreeSurface(instructionsSurf);
    instructionsBox[i].x = 100;
    instructionsBox[i].y = 100 * (i + 1);
    instructionsBox[i].h = 80;
    instructionsBox[i].w = 600;
  }
  
  //dark grey, cooler than black
  SDL_SetRenderDrawColor(rend, 25, 25, 25, 255);
  
  //event loop
  SDL_Event event;
  GameState state = INSTRUCTIONS;
  unsigned int oldTime = 0;
  unsigned int curTime = 0;
  while (state != QUIT){
    switch (state) {
    case INSTRUCTIONS:
      SDL_RenderClear(rend);
      for (int i = 0; i < 4; i++) {
        SDL_RenderCopy(rend, instructionsTex[i], NULL, &(instructionsBox[i]));
      }
      SDL_RenderPresent(rend);
      //get events
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
          state = QUIT;
          break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          state = SHOOTING;
          ship->hitbox->x = 400;
          ship->vel = 0;
          SDL_Delay(300);
          break;
        default:
          break;
        }
      }
      break;
      
    case SHOOTING:
      //update the timer
      curTime = SDL_GetTicks() - oldTime;
      oldTime = SDL_GetTicks();
      //let's draw some stuff
      SDL_RenderClear(rend);
      for (int i = 0; i < WAVELENGTH; i++) {
        if (wave->data[i] != NULL) {
          SDL_RenderCopy(rend, wave->data[i]->tex, NULL, wave->data[i]->hitbox);
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
        scoresurf = TTF_RenderText_Blended(orbitron, scorestring, white);
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
          state = QUIT;
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
            state = QUIT;
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
        //invader/bullet collision
        if(wave->data[i] != NULL && ship->bullet != NULL) {
          if(SDL_HasIntersection(ship->bullet->hitbox, wave->data[i]->hitbox)) {
            score += 1;
            wave->invadersKilled += 1;
            //recalculate invader velocity on a kill
            wave->invaderVel = wave->invaderLR * 0.005 *
              (wave->invadersKilled + 1);
            destroyInvader(wave->data[i]);
            destroyBullet(ship->bullet);
            wave->data[i] = NULL;
            ship->bullet = NULL;
          }
        }
        //invader/ship collisions
        if(wave->data[i] != NULL) {
          if(SDL_HasIntersection(ship->hitbox, wave->data[i]->hitbox)) {
            resetWave(wave);
            state = INSTRUCTIONS;
            break;
          }
        }
      }
      
      /*** Update objects ***/
      //ship stuff
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
      //invader stuff
      for (int i = 0; i < 55; i++) {
        if (wave->data[i] != NULL) {
          if (wave->data[i]->hitbox->x >= 800 - 2 * WIDTH) {
            wave->invaderLR = -1;
            wave->invaderVel = wave->invaderLR * 0.005 *
              (wave->invadersKilled + 1);
          }
          if (wave->data[i]->hitbox->x < WIDTH) {
            wave->invaderLR = 1;
            wave->invaderVel = wave->invaderLR * 0.005 *
              (wave->invadersKilled + 1);
            //move 'em down
            if (wave->movedDown == SDL_FALSE) {
              for (int j = 0; j < 55; j++) {
                if (wave->data[j] != NULL) {
                  wave->data[j]->hitbox->y += WIDTH;
                }
              }
              wave->movedDown = SDL_TRUE;
            }
          }
          wave->data[i]->x += wave->invaderVel * SPEED * curTime;
          wave->data[i]->hitbox->x = (int)wave->data[i]->x;
          if (wave->data[i]->hitbox->y >= 600){
            resetWave(wave);
            state = INSTRUCTIONS;
          }
        }
      }
      wave->movedDown = SDL_FALSE;
      break;
      /*** End Shooting ***/
    default:
      break;
    }
  }
  /* cleanup on aisle here */
  destroyWave(wave);
  destroyShip(ship);
  SDL_FreeRW(text);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
  return 0;
}

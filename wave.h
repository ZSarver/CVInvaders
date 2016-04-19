#ifndef WAVE_H //include guards~~
#define WAVE_H

#include "SDL.h"
#include "SDL_ttf.h"

#include "invader.h"

typedef struct {
  int invadersKilled;
  int invaderCount;
  int invaderLR; //are they moving left or right?
  double invaderVel; /* Invader velocity is collective, and fine-grained
                        enough to warrant sub-pixel accuracy. */
  SDL_bool movedDown; /* need to keep track of whether they've moved down
                         this frame */
  Invader *data[55];
} Wave;

int loadInvaderWave(FILE *text, Wave *nw, SDL_Renderer* rend,
                     TTF_Font* font);
void destroyWave(Wave *iw);
void resetWave(Wave *wa);
void spawnBullets(Wave *wa, SDL_Renderer *rend, TTF_Font *font);

#endif

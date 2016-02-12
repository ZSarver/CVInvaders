#ifndef INVADER_H //frickin' C and it's frickin' include guards
#define INVADER_H

#include "SDL.h"
#include "SDL_ttf.h"

typedef struct {
  SDL_Rect* screenPos;
  SDL_Texture* tex;
} Invader;

//function prototypes
Invader* createInvader(char c, int x, int y, SDL_Renderer* rend, TTF_Font* font);
void destroyInvader(Invader* invader);
int loadInvaderWave(SDL_RWops* file, Invader* waveData[], SDL_Renderer* rend, TTF_Font* font);

#endif

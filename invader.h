#ifndef INVADER_H //frickin' C and it's frickin' include guards
#define INVADER_H

#include "SDL.h"
#include "SDL_ttf.h"

/* Invader movement is fine-grained enough to warrant sub-pixel motion. */
typedef struct {
  SDL_Rect* hitbox;
  SDL_Rect* startingPos;
  SDL_Texture* tex;
  double x;
} Invader;

//function prototypes
Invader* createInvader(char c[], int x, int y, int h, int w,
                       SDL_Renderer* rend, TTF_Font* font);
void destroyInvader(Invader* invader);
void resetInvader(Invader *inv);
#endif

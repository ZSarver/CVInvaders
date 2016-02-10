#ifndef INVADER_H //frickin' C and it's frickin' include guards
#define INVADER_H

#include "SDL.h"

typedef struct {
  int x;
  int y;
  SDL_Texture* tex;
} Invader;

#endif

#include "invader.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>

Invader* createInvader(char c, int x, int y, SDL_Renderer* rend, TTF_Font* font) {
  //allocate memory for invader
  Invader* newInvader = malloc(sizeof(Invader));
  if (newInvader == NULL)
    {
      printf("Cannot allocate memory for new invader!\n");
      exit(1);
    }
  //create new texture
  SDL_Color textColor; //invaders will always be white
  textColor.r = 255;
  textColor.g = 255;
  textColor.b = 255;
  textColor.a = 255;
  SDL_Surface* surf = TTF_RenderText_Blended(font, &c, textColor);
  newInvader->tex = SDL_CreateTextureFromSurface(rend,surf);
  //error checking
  if (newInvader->tex == NULL)
    {
      printf("Error creating new invader texture!\n");
      exit(1);
    }
  //assign others
  newInvader->screenPos = malloc(sizeof(SDL_Rect));
  newInvader->screenPos->x = x;
  newInvader->screenPos->y = y;
  newInvader->screenPos->h = 15;
  newInvader->screenPos->w = 15;
  //cleanup
  SDL_FreeSurface(surf);

  return(newInvader);
}

void destroyInvader(Invader* invader) {
  //kill the texture
  SDL_DestroyTexture(invader->tex);
  //kill the rest
  free(invader->screenPos);
  free(invader);
}

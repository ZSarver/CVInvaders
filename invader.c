#include "invader.h"
#include "common.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>

Invader* createInvader(char c[],
                       int x, int y, int h, int w,
                       SDL_Renderer* rend, TTF_Font* font) {
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
  SDL_Surface* surf = TTF_RenderText_Blended(font, c, textColor);
  if (surf == NULL) {
    printf("Error creating new invader surface! %s\n", SDL_GetError());
    exit(1);
  }
  newInvader->tex = SDL_CreateTextureFromSurface(rend,surf);
  //error checking
  if (newInvader->tex == NULL)
    {
      printf("Error creating new invader texture! %s, %s\n", SDL_GetError(), c);
      exit(1);
    }
  //assign others
  newInvader->hitbox = malloc(sizeof(SDL_Rect));
  newInvader->hitbox->x = x;
  newInvader->hitbox->y = y;
  newInvader->hitbox->h = h;
  newInvader->hitbox->w = w;
  newInvader->startingPos = malloc(sizeof(SDL_Rect));
  newInvader->startingPos->x = x;
  newInvader->startingPos->y = y;
  newInvader->startingPos->h = h;
  newInvader->startingPos->w = w;
  newInvader->x = x;
  //cleanup
  SDL_FreeSurface(surf);

  return(newInvader);
}

void destroyInvader(Invader* invader) {
  //kill the texture
  SDL_DestroyTexture(invader->tex);
  //kill the rest
  free(invader->hitbox);
  free(invader->startingPos);
  free(invader);
}

void resetInvader(Invader *inv) {
  //resets the position of an invader
  inv->hitbox->x = inv->startingPos->x;
  inv->hitbox->y = inv->startingPos->y;
  inv->x = inv->startingPos->x;
}

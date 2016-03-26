#include "ship.h"
#include "common.h"
#include "sdl.h"
#include "SDL_ttf.h"
#include <stdlib.h>
#include <stdio.h>

Bullet* createBullet(int x, int y, SDL_Renderer* rend, TTF_Font* font) {
  //allocate memory for bullet
  Bullet* nb = malloc(sizeof(Bullet));
  if (nb == NULL) {
    printf("Failed to create bullet\n");
    exit(1);
  }
  //create texture
  SDL_Color textColor; //everything is white
  textColor.r = 255;
  textColor.g = 255;
  textColor.b = 255;
  textColor.a = 255;
  SDL_Surface* surf = TTF_RenderText_Blended(font, "|", textColor);
  nb->tex = SDL_CreateTextureFromSurface(rend,surf);

  //error checking
  if (nb->tex == NULL)
    {
      printf("Error creating new invader texture!\n");
      exit(1);
    }
  //assign others
  nb->hitbox = malloc(sizeof(SDL_Rect));
  nb->hitbox->x = x;
  nb->hitbox->y = y;
  nb->hitbox->h = WIDTH;
  nb->hitbox->w = WIDTH;
  //cleanup
  SDL_FreeSurface(surf);

  return(nb);
}


void destroyBullet(Bullet* bullet) {
  //destroy the texture
  SDL_DestroyTexture(bullet->tex);
  //and now everything else
  free(bullet->hitbox);
  free(bullet);
}

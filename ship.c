#include "ship.h"
#include "common.h"
#include "sdl.h"
#include "SDL_ttf.h"
#include <stdlib.h>
#include <stdio.h>

Bullet* createBullet(int x, int y, int vel, SDL_Renderer* rend, TTF_Font* font) {
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
      printf("Error creating bullet texture!\n");
      exit(1);
    }
  //assign others
  nb->hitbox = malloc(sizeof(SDL_Rect));
  nb->hitbox->x = x;
  nb->hitbox->y = y;
  nb->hitbox->h = WIDTH;
  nb->hitbox->w = WIDTH;
  nb->vel = vel; //all velocities will be multiplied by SPEED
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

Ship* createShip(int x, int y, SDL_Renderer* rend, TTF_Font* font) {
  //allocate memory for bullet
  Ship* ns = malloc(sizeof(Ship));
  if (ns == NULL) {
    printf("Failed to create ship\n");
    exit(1);
  }
  //create texture
  SDL_Color textColor; //everything is white
  textColor.r = 255;
  textColor.g = 255;
  textColor.b = 255;
  textColor.a = 255;
  /* For some reason, the character ^ doesn't work. Maybe it isn't a glyph
     in Orbitron. In any case, we'll just render a 'v' and flip it when we
     render to the screen. */
  SDL_Surface* surf = TTF_RenderText_Blended(font, "v", textColor);
  if (surf == NULL) {
    printf("Error creating ship surface! %s\n", SDL_GetError());
    exit(1);
  }
  ns->tex = SDL_CreateTextureFromSurface(rend,surf);
  //error checking
  if (ns->tex == NULL)
    {
      printf("Error creating ship texture! %s\n", SDL_GetError());
      exit(1);
    }
  //assign others
  ns->hitbox = malloc(sizeof(SDL_Rect));
  ns->hitbox->x = x;
  ns->hitbox->y = y;
  ns->hitbox->h = WIDTH;
  ns->hitbox->w = WIDTH;
  ns->bullet = NULL;
  ns->vel = 0; //all velocities will be multiplied by SPEED
  //cleanup
  SDL_FreeSurface(surf);

  return(ns);
}


void destroyShip(Ship* ship) {
  //destroy the texture
  SDL_DestroyTexture(ship->tex);
  //and now everything else
  free(ship->hitbox);
  free(ship);
}

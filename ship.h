#ifndef SHIP_H //oh god these include guards
#define SHIP_H

#include "SDL.h"
#include "SDL_ttf.h"

typedef struct {
  SDL_Rect* hitbox;
  SDL_Texture* tex;
  int vel; //velocity
} Bullet;

typedef struct {
  SDL_Rect* hitbox;
  SDL_Texture* tex;
  Bullet* bullet;
  int vel; //velocity
} Ship;

Bullet* createBullet(int x, int y, int vel, SDL_Renderer* rend, TTF_Font* font);
void destroyBullet(Bullet* bullet);

Ship* createShip(int x, int y, SDL_Renderer* rend, TTF_Font* font);
void destroyShip(Ship* ship);

#endif

#include <stdio.h>
#include <SDL.h>
#undef main //wtf, sdl2

int main(int nArg, char* args)
{
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
    {
      printf("oops you bwoke it \n");
      return 1;
    }
  if (SDL_VideoInit(NULL) != 0)
    {
      printf("oops you bwoke it \n");
      return 1;
    }
  SDL_Window* window  = SDL_CreateWindow("test", 100, 100, 800, 600, 0);
  printf("Oh, hello there.\n");
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

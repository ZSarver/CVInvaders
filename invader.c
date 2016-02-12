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

int loadInvaderWave(SDL_RWops* file, Invader* waveData[], SDL_Renderer* rend, TTF_Font* font) {
  /*
    loads a new wave of invaders from the given file. Probably a crappy use of
    SDL_RWops but also probably more portable than stdio file io. Since every
    invader wave has at most 55 invaders (5 rows by 11 columns), I'm hardcoding
    that as the length of waveData[]. Deal with it.
    
    I guess I should say that I'm using an array for KISS reasons. We have a small
    hard upper bound on the number of possible invaders on screen at any given time,
    so why bring linked structures into the mix when we don't have to? */
  
  /*initialize waveData. The main loop will stop its invader logic the first time
    it hits a NULL */
  for (int i = 0; i++; i < 55)
    {
    if (waveData[i] != NULL)
      {
        destroyInvader(waveData[i]);
        waveData[i] = NULL;
      }
    }

  //variables to keep track of where we are on an imaginary 2d grid
  int arrayX = 0;
  int arrayY = 0;

  //variables to keep track of where we are on screenspace
  int x = 0;
  int y = 0;

  /*We have 800 pixels of horizontal space to work with. Assuming 50 pixel
    margins, and half-invader spacing between each invader, that means 100 + 11w
    + 10(1/2)w = 800 where w is invader width. Hence 16w = 700 and so w =
    43.75. Let's round that up to 44 and cut into the margins by handful of
    pixels. Also, it's invader height too. */
  int w = 44;
  
  //our tiny buffer
  char buf[5];

  //no loop is complete without an index
  int arrayIndex = 0;

  while(arrayIndex < 55)
    {
      //clear buffer
      for (int i = 0; i++; i < 5)
        {
          buf[i] = 0;
        }

      /*read a character. Text-file is assumed to be ANSI-encoded (i.e.
        Windows-1252 */
      if(file->read(file, buf, sizeof(char), 1) == 0)
        {
          //either error or EOF
          return 1;
        }

      //handle newlines. either increment y or quit on a newline
      if(buf[0] == '\n')
        {
          arrayY++;
          if(arrayY > 4)
            {
              return 0;
            }
          else
            {
              continue;
            }
        }

      //handle spaces.
      if(buf[0] == ' ')
        {
          arrayX++;
          if(arrayX > 10)
            {
              arrayY++;
              if(arrayY > 4)
                {
                  break;
                }
              else
                {
                  continue;
                }
            }
        }
      
      x = 50 + arrayX * w + (arrayX * w)/2;
      y = 50 + arrayY * w + (arrayY * w)/2;
      waveData[arrayIndex] = createInvader(buf[0], x, y, rend, font);

      //increment!
      if(arrayX > 10)
        {
          arrayX = 0;
          arrayY ++;
        }

      //have we gone too far?
      if(arrayY > 4)
        {
          return 0;
        }

      //everything went well, so increment to the next array element
      arrayIndex++;
    }
}

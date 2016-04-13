#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "wave.h"
#include "common.h"

int loadInvaderWave(SDL_RWops* file, Wave *nw, SDL_Renderer* rend,
                     TTF_Font* font) {
  /*
    loads a new wave of invaders from the given file. Probably a crappy use of
    SDL_RWops but also probably the most portable, as if I care about
    that. Since every invader wave has at most 55 invaders (5 rows by 11
    columns), I'm hardcoding that as the length of waveData[]. Deal with it.
    
    I guess I should say that I'm using an array for KISS reasons. We have a
    small hard upper bound on the number of possible invaders on screen at any
    given time, so why bring linked structures into the mix when we don't have
    to? */

  nw->invadersKilled = 0;
  nw->invaderCount = 0;
  nw->invaderLR = 1; //left is -1 right is 1
  nw->invaderVel = nw->invaderLR * 0.005 * (nw->invadersKilled + 1);
  nw->movedDown = SDL_FALSE;
  
  /*initialize waveData. The main loop will stop its invader logic the first time
    it hits a NULL */
  for (int i = 0; i++; i < 55)
    {
    if (nw->data[i] != NULL)
      {
        destroyInvader(nw->data[i]);
        nw->data[i] = NULL;
      }
    }

  //variables to keep track of where we are on an imaginary 2d grid
  int arrayX = 0;
  int arrayY = 0;

  //varieables to keep track of where we are on screenspace
  int x = 0;
  int y = 0;

  /*We have 800 pixels of horizontal space to work with. Assuming 50 pixel
    margins, and half-invader spacing between each invader, that means 100 + 11w
    + 10(1/2)w = 800 where w is invader width. Hence 16w = 700 and so w =
    43.75. Let's round that up to 44 and cut into the margins by handful of
    pixels. Also, it's invader height too. */
  int w = WIDTH;
  
  //our tiny buffer
  char buf[1];

  for(int arrayIndex = 0; arrayIndex < 55; arrayIndex++) {
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
    if(buf[0] == '\n') {
      arrayY++;
      if(arrayY > 4) {
        return 0;
      }
      continue;
      }
    
    //handle spaces.
    if(buf[0] == ' ') {
      arrayX++;
      if(arrayX > 10) {
        arrayY++;
        arrayX = 0;
        if(arrayY > 4) {
          return 0;
        }
      }
      continue;
    }
    
    x = 50 + arrayX * w + (arrayX * w)/2;
    y = 50 + arrayY * w + (arrayY * w)/2;
    nw->data[arrayIndex] = createInvader(buf, x, y, w, w, rend, font);
    nw->invaderCount += 1;
    
    //increment!
    if(arrayX++ > 10) {
      arrayX = 0;
      arrayY++;
    }
    
    //have we gone too far?
    if(arrayY > 4) {
      return 0;
    }
  }
  return 0;
}

void destroyWave(Wave *iw) {
  //kill the data
  for (int i = 0; i < 55; i++) {
    if (iw->data[i] != NULL) {
      destroyInvader(iw->data[i]);
      free(iw->data[i]);
    }
  }
  free(iw);
}

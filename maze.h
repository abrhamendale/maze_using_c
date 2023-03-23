#ifndef MAZE
#define MAZE
#ifdef _WIN32a
#include <SDL.h>   /* All SDL App's need this */
#else
#include <time.h>//<linux/time.h>
#define __timespec_defined 1
#define __timeval_defined 1
#define __itimerspec_defined 1
#include "SDL2/SDL.h"
#endif
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define QUITKEY SDLK_ESCAPE
#define WIDTH 1024
#define HEIGHT 768

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
void loader(double *p1, unsigned int *p2, unsigned int w, unsigned int h);
#endif

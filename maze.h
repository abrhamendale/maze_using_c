#ifndef MAZE
#define MAZE
#ifdef _WIN32a
#include <SDL.h>
#else
#include <time.h>
#define __timespec_defined 1
#define __timeval_defined 1
#define __itimerspec_defined 1
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#endif
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define QUITKEY SDLK_ESCAPE
#define WIDTH 1024
#define HEIGHT 768

#define screenWidth 640
#define screenHeight 480

#define mapWidth 24
#define mapHeight 24

#define texWidth 64
#define texHeight 64

extern int worldMap[mapWidth][mapHeight];
/*
 *{
 *{4,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
 *{4,0,0,0,4,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
 *{4,0,6,0,4,0,0,6,0,0,0,0,0,0,0,7,0,4,4,4,4,4,4,7},
 *{4,0,6,0,4,0,0,6,7,7,7,7,0,0,7,0,0,0,0,0,0,0,0,7},
 *{4,0,6,0,4,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
 *{4,0,6,0,4,0,0,6,6,6,6,6,6,6,6,6,7,7,0,0,0,0,0,7},
 *{4,0,6,0,4,0,0,6,0,6,0,6,0,6,0,6,7,0,0,0,7,7,7,7},
 *{4,0,6,0,4,0,0,6,0,0,0,0,0,0,0,6,7,0,0,0,0,0,0,8},
 *{4,0,6,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,7,7,7,8},
 *{4,0,6,6,6,6,6,6,0,0,6,0,0,0,0,6,7,0,0,0,0,0,0,8},
 *{4,0,0,0,0,0,0,6,0,0,0,0,0,0,0,6,7,0,0,0,7,7,7,8},
 *{4,0,0,0,0,0,0,6,6,6,6,0,0,0,0,6,7,0,0,0,0,0,0,8},
 *{6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
 *{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
 *{6,6,6,6,6,6,0,6,6,6,6,0,6,0,0,0,0,0,0,0,0,0,0,6},
 *{4,4,4,4,4,4,0,4,4,4,6,0,6,3,3,3,3,3,3,3,3,3,3,3},
 *{4,0,0,0,0,0,0,0,0,4,6,0,6,4,0,0,0,0,0,3,0,0,0,3},
 *{4,2,2,2,2,2,2,2,0,0,0,0,4,0,0,6,0,0,3,0,0,0,0,3},
 *{4,0,0,0,0,0,0,0,0,4,6,0,6,4,0,0,0,0,0,3,3,0,3,3},
 *{4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,6,0,0,0,0,0,0,3},
 *{4,0,0,0,0,0,0,0,0,0,0,0,6,4,0,0,0,0,0,3,3,0,3,3},
 *{4,0,6,0,6,0,0,0,0,4,6,0,6,4,0,0,5,0,0,3,0,0,0,3},
 *{4,0,0,0,0,0,0,0,0,4,6,0,6,4,0,0,0,0,0,3,0,0,0,3},
 *{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3}
 *};
 */

extern double buffer[screenHeight][screenWidth];
extern double texture[8][texWidth * texHeight];

/**
 * struct playerpos - A structure for the player's orientation
 *
 * @posX: X position
 * @posY: Y position
 * @dirX: X direction
 * @dirY: Y direction
 * @planeX: Camera plane x direction
 * @planeY: camera plane y direction
 * @time: Current time
 * @oldTime: last iteration time
 */
struct playerpos
{
	double posX;
	double posY;
	double dirX;
	double dirY;
	double planeX;
	double planeY;
	double time;
	double oldTime;
};
typedef struct playerpos player;

/**
 * struct bufarray - An structure to store the pixels
 *
 * @buf: An array to store the pixel data
 */
struct bufarray
{
	double buf[480][640];
};
typedef struct bufarray ba;

/**
 * struct ray - A structure to store ray orientations
 *
 * @rayDirX: Ray x direction
 * @rayDirY: Ray y direction
 * @sideDistX: Initial x distance
 * @sideDistY: Initial y distance
 * @deltaDistX: Incremental x distance
 * @deltaDistY: Incremental y distance
 * @stepX: A step in the map horizontal direction
 * @stepY: A step in the map vertical direction
 * @mapX: Current map x position
 * @mapY: Current map y position
 * @side: Side
 *
 */
struct ray
{
	double rayDirX;
	double rayDirY;
	double sideDistX;
	double sideDistY;
	double deltaDistX;
	double deltaDistY;
	int stepX;
	int stepY;
	int mapX;
	int mapY;
	int side;
};

/**
 * struct App - Listens to keyboard presses
 *
 * @renderer: SDL_renderer object
 * @window: SDL_window object
 * @up: key up
 * @down: key down
 * @left: key left
 * @right: key right
 *
 */
struct App
{
	SDL_Renderer *renderer;
	SDL_Window *window;
	int up;
	int down;
	int left;
	int right;
};
struct App app;
SDL_Window *initw();
void loader(double *p1, unsigned int *p2, unsigned int w, unsigned int h);
void texloader(SDL_Surface *screenSurface);
void renderfloor(struct playerpos *player, struct bufarray *buffer);
void dda(struct playerpos *player, struct ray *r, int x);
void doInput(SDL_Event *event);
void doKeyDown(SDL_KeyboardEvent *event);
void doKeyUp(SDL_KeyboardEvent *event);
void playermov(struct playerpos *player, double moveSpeed, double rotSpeed);
void texpositioner(struct ray *r, struct playerpos *player
		, int *drawstart, int *drawEnd, int *texNum, int *lineHeight, int *texX);
void playerturn(struct playerpos *player, double *rotSpeed
		, double *oldDirX, double *oldPlaneX);
void renderwall(struct playerpos *player);
#endif

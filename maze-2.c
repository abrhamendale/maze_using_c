#include "maze.h"
//#include <SDL2/SDL_image.h>

/*
worldMap[mapWidth][mapHeight] =
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};
*/

void doInput(SDL_Event *event)
{
	while (SDL_PollEvent(event))
	{
		switch (event->type)
		{
			case SDL_QUIT:
				exit(0);
				break;

			case SDL_KEYDOWN:
				doKeyDown(&event->key);
				break;

			case SDL_KEYUP:
				doKeyUp(&event->key);
				break;

			default:
				break;
		}
	}
}

void doKeyDown(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
			app.up = 1;

		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
			app.down = 1;

		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
			app.left = 1;

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
			app.right = 1;
	}
}

void doKeyUp(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
			app.up = 0;

		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
			app.down = 0;

		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
			app.left = 0;

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
			app.right = 0;
	}
}

SDL_Window *initw()
{
	SDL_Window *window = NULL;
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	else
	{
		window = SDL_CreateWindow("SDL Tutorial!", SDL_WINDOWPOS_UNDEFINED
				, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight
				, SDL_WINDOW_SHOWN);
		if(window == NULL)
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		else
			return (window);
        }
	return (NULL);
}

void loader(double *p1, unsigned int *p2, unsigned int w, unsigned int h)
{
	unsigned int x = 0, y = 0;

	for (y = 0 ; y < h ; ++y)
		for (x = 0 ; x < w ; ++x)
			p1[x + w * y] = p2[w * y + x];
}

void playermov(struct playerpos *player, double moveSpeed, double rotSpeed)
{
	double oldDirX = 0, oldPlaneX = 0;
        SDL_Event e;

	while (1)
	{
		doInput(&e);
		if (app.up)
		{
			if(worldMap[(int)(player->posX + player->dirX * moveSpeed
                                                )][(int)(player->posY)] == 0)
                                player->posX += player->dirX * moveSpeed;
                        if(worldMap[(int)(player->posX)][(int)(player->posY
                                                + player->dirY * moveSpeed)] == 0)
                                player->posY += player->dirY * moveSpeed;
                }

	                if (app.down)
                {
                        if (worldMap[(int)(player->posX - player->dirX
                                                * moveSpeed)][(int)(player->posY)] == 0)
                                player->posX -= player->dirX * moveSpeed;
                        if (worldMap[(int)player->posX][(int)(player->posY
                                                - player->dirY * moveSpeed)] == 0)
                                player->posY -= player->dirY * moveSpeed;
                }

                if (app.right)
                {
                        oldDirX = player->dirX;
                        player->dirX = player->dirX * cos(-rotSpeed) - player->dirY * sin(-rotSpeed);
                        player->dirY = oldDirX * sin(-rotSpeed) + player->dirY * cos(-rotSpeed);
                        oldPlaneX = player->planeX;
                        player->planeX = player->planeX * cos(-rotSpeed)
                                - player->planeY * sin(-rotSpeed);
                        player->planeY = oldPlaneX * sin(-rotSpeed) + player->planeY
                                * cos(-rotSpeed);
                }

                if (app.left)
                {
                        oldDirX = player->dirX;
                        player->dirX = player->dirX * cos(rotSpeed) - player->dirY * sin(rotSpeed);
                        player->dirY = oldDirX * sin(rotSpeed) + player->dirY * cos(rotSpeed);
                        oldPlaneX = player->planeX;
                        player->planeX = player->planeX * cos(rotSpeed) - player->planeY * sin(rotSpeed);
                        player->planeY = oldPlaneX * sin(rotSpeed) + player->planeY
                                * cos(rotSpeed);
                }
		break;
	}
}

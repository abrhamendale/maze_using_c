#include "maze.h"

/**
 * initw - Initializes the SDL window
 *
 *
 * Return: Nothing
 */
SDL_Window *initw()
{
	SDL_Window *window = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL could not initialize! SDL_Error: %s\n"
				, SDL_GetError());
	else
	{
		window = SDL_CreateWindow("!!! Maze !!!", SDL_WINDOWPOS_UNDEFINED
				, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight
				, SDL_WINDOW_SHOWN);
		if (window == NULL)
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		else
			return (window);
	}
	return (NULL);
}

/**
 * loader - Copies one array into another
 *
 * @p1: Array one
 * @p2: Array two
 * @w: The width of the screen
 * @h: The height of the screen
 *
 * Return: Nothing
 */
void loader(double *p1, unsigned int *p2, unsigned int w, unsigned int h)
{
	unsigned int x = 0, y = 0;

	for (y = 0 ; y < h ; ++y)
		for (x = 0 ; x < w ; ++x)
			p1[x + w * y] = p2[w * y + x];
}

/**
 * texloader - loades textures from images
 *
 * @screenSurface: The surface to render on
 *
 * Return: Nothing
 */
void texloader(SDL_Surface *screenSurface)
{
	SDL_Surface *tloader;

	tloader = IMG_Load("pics/92.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[0], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/75.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[1], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/8.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[2], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/8.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[3], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/177.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[4], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/177.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[5], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/74.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[6], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/177.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[7], tloader->pixels, tloader->w, tloader->h);
}

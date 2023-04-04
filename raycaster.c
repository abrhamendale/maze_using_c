#include "maze.h"
#include <SDL2/SDL_image.h>

/**
 * texpositioner - Determines the position of the texture to render
 *
 * @r: A struct which holds the orientation of the rays
 * @player: A struct which holds the orientation of the players
 * @drawStart: A variable to hold the starting position of rendering
 * @drawEnd: A variable to hold the end position of the rendering
 * @texNum: A variable which holds which texture to render
 * @lineHeight: The calculated height to render
 * @texX: The x position of the texture
 *
 * Return: Nothing
 */
void texpositioner(struct ray *r, struct playerpos *player, int *drawStart
		, int *drawEnd, int *texNum, int *lineHeight, int *texX)
{
	double perpWallDist = 0;
	int h = screenHeight;

	if (r->side == 0)
		perpWallDist = (r->sideDistX - r->deltaDistX);
	else
		perpWallDist = (r->sideDistY - r->deltaDistY);
	(*lineHeight) = (int)(h / perpWallDist);
	(*drawStart) = -(*lineHeight) / 2 + h / 2;
	if (*drawStart < 0)
		*drawStart = 0;
	*drawEnd = (*lineHeight) / 2 + h / 2;
	if (*drawEnd >= h)
		*drawEnd = h - 1;
	(*texNum) = worldMap[r->mapX][r->mapY] - 1;
	double wallX;

	if (r->side == 0)
		wallX = player->posY + perpWallDist * (r->rayDirY);
	else
		wallX = player->posX + perpWallDist * (r->rayDirX);
	wallX -= floor(wallX);
	(*texX) = (int)(wallX * (double)texWidth);
	if (r->side == 0 && r->rayDirX > 0)
		(*texX) = texWidth - (*texX) - 1;
	if (r->side == 1 && r->rayDirY < 0)
	(*texX) = texWidth - (*texX) - 1;
}

/**
 * dda - Algorithm to find the intersection of a ray with a wall
 *
 * @player: A struct to hold the orientation of the player
 * @r: A struct to hold the orientation of the rays
 * @x: A variable to hold the present vertical strip
 *
 * Return: Nothing
 */
void dda(struct playerpos *player, struct ray *r, int x)
{
	int hit = 0, h = screenHeight, texX = 0, lineHeight = 0;
	int texNum = 0, drawStart = 0, drawEnd = 0;

	while (hit == 0)
	{
		if (r->sideDistX < r->sideDistY)
		{
			r->sideDistX += r->deltaDistX;
			r->mapX += r->stepX;
			r->side = 0;
		}
		else
		{
			r->sideDistY += r->deltaDistY;
			r->mapY += r->stepY;
			r->side = 1;
		}
		if (worldMap[r->mapX][r->mapY] > 0)
			hit = 1;
	}
	texpositioner(r, player, &drawStart, &drawEnd, &texNum, &lineHeight, &texX);

	double step = 1.0 * texHeight / lineHeight;
	double texPos = (drawStart - h / 2 + lineHeight / 2) * step;

	for (int y = drawStart; y < drawEnd; y++)
	{
		int texY = (int)texPos & (texHeight - 1);

		texPos += step;
		Uint32 color = texture[texNum][texHeight * texY + texX];

		if (r->side == 1)
			color = (color >> 1) & 8355711;
		buffer[y][x] = color;
	}
}

/**
 * renderwall - Determines the position of the texture to render
 *
 * @player: A struct which holds the orientation of the player
 *
 * Return: Nothing
 */
void renderwall(struct playerpos *player)
{
	int w = screenWidth;
	struct ray r;

	for (int x = 0; x < w; x++)
	{
		double cameraX = 2 * x / (double)w - 1;

		r.rayDirX = player->dirX + (player->planeX) * cameraX;
		r.rayDirY = player->dirY + (player->planeY) * cameraX;
		r.mapX = (int)(player->posX);
		r.mapY = (int)(player->posY);

		r.deltaDistX = (r.rayDirX == 0) ? 1e30 : fabs(1 / r.rayDirX);
		r.deltaDistY = (r.rayDirY == 0) ? 1e30 : fabs(1 / r.rayDirY);

		if (r.rayDirX < 0)
		{
			r.stepX = -1;
			r.sideDistX = (player->posX - r.mapX) * (r.deltaDistX);
		}
		else
		{
			r.stepX = 1;
			r.sideDistX = (r.mapX + 1.0 - player->posX) * r.deltaDistX;
		}
		if (r.rayDirY < 0)
		{
			r.stepY = -1;
			r.sideDistY = (player->posY - r.mapY) * (r.deltaDistY);
		}
		else
		{
			r.stepY = 1;
			r.sideDistY = (r.mapY + 1.0 - player->posY) * (r.deltaDistY);
		}
		dda(player, &r, x);
	}
}

/**
 * renderfloor - Renders the floor and roof
 *
 * @player: A struct to hold player position data
 * @buffer: A 2x2 array to hold the pixels to be rendered
 *
 * Return: Nothing
 */
void renderfloor(struct playerpos *player, struct bufarray *buffer)
{
	int h = screenHeight;

	for (int y = 0; y < h; y++)
	{
		float rayDirX0 = player->dirX - player->planeX;
		float rayDirY0 = player->dirY - player->planeY;
		float rayDirX1 = player->dirX + player->planeX;
		float rayDirY1 = player->dirY + player->planeY;
		int p = y - screenHeight / 2;
		double posZ = 0.5 * screenHeight;
		float rowDistance = posZ / p;
		float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
		float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;
		float floorX = player->posX + rowDistance * rayDirX0;
		float floorY = player->posY + rowDistance * rayDirY0;

		for (int x = 0; x < screenWidth; ++x)
		{
			int cellX = (int)(floorX);
			int cellY = (int)(floorY);
			int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
			int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

			floorX += floorStepX;
			floorY += floorStepY;

			int floorTexture = 0;
			int ceilingTexture = 1;
			Uint32 color;

			color = texture[floorTexture][texWidth * ty + tx];
			color = (color >> 1) & 8355711;
			(buffer->buf)[y][x] = color;
			color = texture[ceilingTexture][texWidth * ty + tx];
			color = (color >> 1) & 8355711;
			(buffer->buf)[screenHeight - y - 1][x] = color;
		}
	}
}

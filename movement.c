#include "maze.h"

/**
 * playerturn - Rotates the player with key presses
 *
 * @player: A struct which holds the orientation of the players
 * @rotSpeed: Rotation speed
 * @oldDirX: Old direction x component
 * @oldPlaneX: Old direction plane x component
 *
 * Return: Nothing
 */
void playerturn(struct playerpos *player, double *rotSpeed
		, double *oldDirX, double *oldPlaneX)
{
	if (app.right)
	{
		*oldDirX = player->dirX;
		player->dirX = player->dirX * cos(-*rotSpeed)
			- player->dirY * sin(-*rotSpeed);
		player->dirY = (*oldDirX) * sin(-*rotSpeed) + player->dirY * cos(-*rotSpeed);
		*oldPlaneX = player->planeX;
		player->planeX = player->planeX * cos(-*rotSpeed)
				- player->planeY * sin(-*rotSpeed);
		player->planeY = *oldPlaneX * sin(-(*rotSpeed)) + player->planeY
				* cos(-*rotSpeed);
	}
	if (app.left)
	{
		*oldDirX = player->dirX;
		player->dirX = player->dirX * cos(*rotSpeed) - player->dirY * sin(*rotSpeed);
		player->dirY = *oldDirX * sin(*rotSpeed) + player->dirY * cos(*rotSpeed);
		*oldPlaneX = player->planeX;
		player->planeX = player->planeX * cos(*rotSpeed)
			- player->planeY * sin(*rotSpeed);
		player->planeY = *oldPlaneX * sin(*rotSpeed) + player->planeY
				* cos(*rotSpeed);
	}
}

/**
 * playermov - Moves the player with key presses
 *
 * @player: A struct which holds the orientation of the players
 * @moveSpeed: The speed of movment
 * @rotSpeed: The speed of rotation
 *
 * Return: Nothing
 */
void playermov(struct playerpos *player, double moveSpeed
		, double rotSpeed)
{
	double oldDirX = 0, oldPlaneX = 0;
	SDL_Event e;

	while (1)
	{
		doInput(&e);
		if (app.up)
		{
			if (worldMap[(int)(player->posX + player->dirX * moveSpeed
						)][(int)(player->posY)] == 0)
				player->posX += player->dirX * moveSpeed;
			if (worldMap[(int)(player->posX)][(int)(player->posY
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
		if (app.right || app.left)
			playerturn(player, &rotSpeed, &oldDirX, &oldPlaneX);
		break;
	}
}

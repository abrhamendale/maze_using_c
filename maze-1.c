#include "maze.h"
#include <SDL2/SDL_image.h>
//#define screenWidth 640
//#define screenHeight 480

#define texWidth 64
#define texHeight 64

double buffer[screenHeight][screenWidth];
double texture[8][texWidth * texHeight];

/*
int worldMap[mapWidth][mapHeight] =
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

void dda(struct playerpos *player, struct ray *r, int x)
{
	double perpWallDist;
	int hit = 0, h = screenHeight;
	
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
	if(r->side == 0)
		perpWallDist = (r->sideDistX - r->deltaDistX);
	else
		perpWallDist = (r->sideDistY - r->deltaDistY);
	int lineHeight = (int)(h / perpWallDist);
	int drawStart = -lineHeight / 2 + h / 2;
	if(drawStart < 0)
		drawStart = 0;
	int drawEnd = lineHeight / 2 + h / 2;
	if(drawEnd >= h)
		drawEnd = h - 1;
	int texNum = worldMap[r->mapX][r->mapY] - 1;
	double wallX; //where exactly the wall was hit
	if (r->side == 0)
		wallX = player->posY + perpWallDist * (r->rayDirY);
	else
		wallX = player->posX + perpWallDist * (r->rayDirX);
	wallX -= floor(wallX);
	int texX = (int)(wallX * (double)texWidth);
	if(r->side == 0 && r->rayDirX > 0)
		texX = texWidth - texX - 1;
	if(r->side == 1 && r->rayDirY < 0)
		texX = texWidth - texX - 1;
	double step = 1.0 * texHeight / lineHeight;
	double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
	for(int y = drawStart; y<drawEnd; y++)
	{
		int texY = (int)texPos & (texHeight - 1);
		texPos += step;
		Uint32 color = texture[texNum][texHeight * texY + texX];
		if(r->side == 1)
			color = (color >> 1) & 8355711;
		buffer[y][x] = color;
	}
}

void renderwall(struct playerpos *player)
{
	int w = screenWidth;
	struct ray r;

	for(int x = 0; x < w; x++)
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

void renderfloor(struct playerpos *player, struct bufarray *buffer)
{
	int h = screenHeight;

	for(int y = 0; y < h; y++)
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

		for(int x = 0; x < screenWidth; ++x)
		{
			int cellX = (int)(floorX);
			int cellY = (int)(floorY);
			int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
			int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);
			floorX += floorStepX;
			floorY += floorStepY;
			int floorTexture = 3;
			int ceilingTexture = 7;
			Uint32 color;
			color = texture[floorTexture][texWidth * ty + tx];
			color = (color >> 1) & 8355711; // make a bit darker
			(buffer->buf)[y][x] = color;
			color = texture[ceilingTexture][texWidth * ty + tx];
			color = (color >> 1) & 8355711; // make a bit darker
			(buffer->buf)[screenHeight - y - 1][x] = color;
		}
	}
}

void texloader(SDL_Surface *screenSurface)
{
	SDL_Surface *tloader;
	tloader = IMG_Load("pics/mossy.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[0], tloader->pixels, tloader->w, tloader->h);
	printf("SEG!!!\n");
	tloader = IMG_Load("pics/redwall.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	printf("Screen surface Format: %u\n", screenSurface->format->format);
	printf("Image_Load Format: %u\n", tloader->format->format);
	loader(texture[1], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/purplestone.png");
	//tloader = IMG_Load("pics/purplestone.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[2], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/colorfloor.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[3], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/redbrick.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[4], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/redwall.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[5], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/colorfloor.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[6], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/wood.png");
	tloader = SDL_ConvertSurface(tloader, screenSurface->format, 0);
	loader(texture[7], tloader->pixels, tloader->w, tloader->h);
}

int main()//int argc, char* args[])
{
	double w = screenWidth, h = screenHeight;
	double time = 0, oldTime = 0;//, planeX = 0, planeY = 0.66;
	int quit = 0;
	SDL_Window* window = initw();
	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
	struct playerpos player;
	struct bufarray buffer_array;

	player.posX = 21, player.posY = 12, player.dirX = -1, player.dirY = 0;
	player.planeX = 0, player.planeY = 0.66, player.time = 0, player.oldTime = 0;
	texloader(screenSurface);
	while (quit == 0)
	{
		renderfloor(&player, &buffer_array);
		for (int i = 0 ; i < screenHeight ; i++)
			for(int j = 0 ; j < screenWidth ; j++)
				buffer[i][j] = (buffer_array.buf)[i][j];
		renderwall(&player);
		unsigned int * pixels = screenSurface->pixels;
		int width = screenSurface->w;
		int height = screenSurface->h;
		for (int y = 0; y < height - 1; ++y)
		{
			for (int x = 0; x < width - 1; ++x)
			{
				if (x > 640 || y > 640)
					pixels[x + y * width] = SDL_MapRGBA(screenSurface->format, 200, 50, 50, 255);
				else
					pixels[x + y * width] = (unsigned int)(buffer[y][x]);
			}
		}
		SDL_UpdateWindowSurface(window);
		for(int y = 0; y < h; y++)
			for(int x = 0; x < w; x++)
				buffer[y][x] = 0;
		oldTime = time;
		time = SDL_GetTicks();
		double frameTime = (time - oldTime) / 1000.0;
		double moveSpeed = frameTime * 1.2;//5.0
		double rotSpeed = frameTime * 1.2;//3.0
		playermov(&player, moveSpeed, rotSpeed);
	}
}

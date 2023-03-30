#include "maze.h"
#include <SDL2/SDL_image.h>
#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

#define texWidth 64
#define texHeight 64

double buffer[screenHeight][screenWidth];
double texture[8][texWidth * texHeight];

int worldMap[mapWidth][mapHeight]=
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

void doInput(SDL_Event *event)
{
	while (SDL_PollEvent(event))
	{
		printf("-------------------------doInput!!!--------------------%u\n", event->type);
		switch (event->type)
		{
			case SDL_QUIT:
				exit(0);
				break;

			case SDL_KEYDOWN:
				printf("----------------doKeyDowndoInput!!!---------------\n");
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
	printf("DoKeyDown!!!%u\n", event->keysym.scancode);
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			app.up = 1;
		}

		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		{
			app.down = 1;
		}

		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		{
			app.left = 1;
		}

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			app.right = 1;
		}
	}
}

void doKeyUp(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			app.up = 0;
		}

		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		{
			app.down = 0;
		}

		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		{
			app.left = 0;
		}

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			app.right = 0;
		}
	}
}

void dda(struct playerpos *player, struct ray *r, int x)
{
	double perpWallDist;
	int hit = 0, h = screenHeight;
	
	//printf("DDA!!!\n");
	while (hit == 0)
	{
		//jump to next map square, either in x-direction, or in y-direction
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
		//Check if ray has hit a wall
		if (worldMap[r->mapX][r->mapY] > 0)
			hit = 1;
	}
	//printf("Hit!!!\n");
	//Calculate distance projected on camera
	//direction (Euclidean distance would give
	//fisheye effect!)
	if(r->side == 0)
		perpWallDist = (r->sideDistX - r->deltaDistX);
	else
		perpWallDist = (r->sideDistY - r->deltaDistY);
	//Calculate height of line to draw on screen
	int lineHeight = (int)(h / perpWallDist);

	//calculate lowest and highest pixel to
	//fill in current stripe
	int drawStart = -lineHeight / 2 + h / 2;
	if(drawStart < 0)
		drawStart = 0;
	int drawEnd = lineHeight / 2 + h / 2;
	if(drawEnd >= h)
		drawEnd = h - 1;

	//texturing calculations
	int texNum = worldMap[r->mapX][r->mapY] - 1;
	//1 subtracted from it so that texture 0 can be used!

	//calculate value of wallX
	double wallX; //where exactly the wall was hit
	if (r->side == 0)
		wallX = player->posY + perpWallDist * (r->rayDirY);
	else
		wallX = player->posX + perpWallDist * (r->rayDirX);
	wallX -= floor(wallX);

	//x coordinate on the texture
	int texX = (int)(wallX * (double)texWidth);
	if(r->side == 0 && r->rayDirX > 0)
		texX = texWidth - texX - 1;
	if(r->side == 1 && r->rayDirY < 0)
		texX = texWidth - texX - 1;

	// How much to increase the texture coordinate per screen pixel
	double step = 1.0 * texHeight / lineHeight;
	// Starting texture coordinate
	double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
	for(int y = drawStart; y<drawEnd; y++)
	{
		// Cast the texture coordinate to integer, 
		// and mask with (texHeight - 1) in case of overflow
		int texY = (int)texPos & (texHeight - 1);
		texPos += step;
		Uint32 color = texture[texNum][texHeight * texY + texX];
		//make color darker for y-sides: R, G and B byte each 
		//divided through two with a "shift" and an "and"
		if(r->side == 1)
			color = (color >> 1) & 8355711;
		buffer[y][x] = color;
		//printf("SEG%d %d %f ", x, y, buffer[y][x]);
	}
}



void renderwall(struct playerpos *player)
{
	int w = screenWidth;
	struct ray r;

	//printf("Renderwall!!!\n");
	for(int x = 0; x < w; x++)
	{
		//calculate ray position and direction
		double cameraX = 2 * x / (double)w - 1;
		//x-coordinate in camera space
		r.rayDirX = player->dirX + (player->planeX) * cameraX;
		r.rayDirY = player->dirY + (player->planeY) * cameraX;
		//which box of the map we're in
		r.mapX = (int)(player->posX);
		r.mapY = (int)(player->posY);

		//length of ray from current position 
		//to next x or y-side
		//double sideDistX;
		//double sideDistY;

		//length of ray from one x or y-side to next x or y-side
		r.deltaDistX = (r.rayDirX == 0) ? 1e30 : fabs(1 / r.rayDirX);
		r.deltaDistY = (r.rayDirY == 0) ? 1e30 : fabs(1 / r.rayDirY);
		//double perpWallDist;

		//to step in x or y-direction (either +1 or -1)
		//int stepX;
		//int stepY;

		//int hit = 0; //was there a wall hit?
		//int side; //was a NS or a EW wall hit?

		//calculate step and initial sideDist
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

void renderfloor(struct playerpos *player, struct bufarray *buffer)//, double texture[][texWidth * texHeight])
{
	int h =SCREEN_HEIGHT;
	//struct bufarray floorbuffer;
	//printf("Floor renderer!!!%f\n", player->posX);
	for(int y = 0; y < h; y++)
	{
		float rayDirX0 = player->dirX - player->planeX;
		float rayDirY0 = player->dirY - player->planeY;
		float rayDirX1 = player->dirX + player->planeX;
		float rayDirY1 = player->dirY + player->planeY;
		int p = y - SCREEN_HEIGHT / 2;
		double posZ = 0.5 * SCREEN_HEIGHT;
		float rowDistance = posZ / p;
		float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;
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
			(buffer->buf)[SCREEN_HEIGHT - y - 1][x] = color;
			//(floorbuffer.buf)[SCREEN_HEIGHT - y - 1][x] = color;
			//printf("Buffer %d %d:%f", y, x, (buffer->buf)[SCREEN_HEIGHT - y - 1][x]);
		}
	}
	//printf("For loop\n");
	/*
	for (int i = 0 ; i < 300 ; i++)
		for(int j = 0 ; j < 300 ; j++)
			(floorbuffer.buf)[i][j] = buffer[i][j];
	*/
	//return (floorbuffer);
}

SDL_Window *initw()
{
	SDL_Window *window = NULL;
	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	else
	{
		//Create window
		window = SDL_CreateWindow("SDL Tutorial!", SDL_WINDOWPOS_UNDEFINED
				, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT
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

void texloader(SDL_Surface *screenSurface)
//void texloader(double p[][4096], SDL_Surface *screenSurface)
{
	SDL_Surface *tloader;
	//double p[8][texWidth * texHeight];
	//p = texture;

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

void playermov(struct playerpos *player, double moveSpeed, double rotSpeed)
{
	double oldDirX = 0, oldPlaneX = 0;
        SDL_Event e;

	//printf("playermov!!!\n");	
	while (1)//SDL_PollEvent(&e) != 0)
	{
		doInput(&e);
		printf("App.up%d\n", app.left);
		if (app.up)
		{
			//move forward if no wall in front of you
                        //printf("UP\n");
			if(worldMap[(int)(player->posX + player->dirX * moveSpeed
                                                )][(int)(player->posY)] == 0)
                                player->posX += player->dirX * moveSpeed;
                        if(worldMap[(int)(player->posX)][(int)(player->posY
                                                + player->dirY * moveSpeed)] == 0)
                                player->posY += player->dirY * moveSpeed;
                }

	                if (app.down)
                {
                        //move backwards if no wall behind you
                        if (worldMap[(int)(player->posX - player->dirX
                                                * moveSpeed)][(int)(player->posY)] == 0)
                                player->posX -= player->dirX * moveSpeed;
                        if (worldMap[(int)player->posX][(int)(player->posY
                                                - player->dirY * moveSpeed)] == 0)
                                player->posY -= player->dirY * moveSpeed;
                }

                if (app.left)
                {
                        //rotate to the right
                        //camera direction and camera plane must be rotated
                        oldDirX = player->dirX;
                        player->dirX = player->dirX * cos(-rotSpeed) - player->dirY * sin(-rotSpeed);
                        player->dirY = oldDirX * sin(-rotSpeed) + player->dirY * cos(-rotSpeed);
                        oldPlaneX = player->planeX;
                        player->planeX = player->planeX * cos(-rotSpeed)
                                - player->planeY * sin(-rotSpeed);
                        player->planeY = oldPlaneX * sin(-rotSpeed) + player->planeY
                                * cos(-rotSpeed);
			printf("--------------------------Move left!!!\n");
                }

                if (app.right)
                {
                        //rotate to the left
                        //both camera direction and camera plane must be rotate
                        oldDirX = player->dirX;
                        player->dirX = player->dirX * cos(rotSpeed) - player->dirY * sin(rotSpeed);
                        player->dirY = oldDirX * sin(rotSpeed) + player->dirY * cos(rotSpeed);
                        oldPlaneX = player->planeX;
                        player->planeX = player->planeX * cos(rotSpeed) - player->planeY * sin(rotSpeed);
                        player->planeY = oldPlaneX * sin(rotSpeed) + player->planeY
                                * cos(rotSpeed);
                }

                //blit(player.texture, player.x, player.y);

                //presentScene();

                //SDL_Delay(16);
	}
}



//double buffer[screenHeight][screenWidth];
//double texture[8][texWidth * texHeight];

int main()//int argc, char* args[])
{
	//double posX = 22, posY = 12, dirX = -1, dirY = 0;
	double w = SCREEN_WIDTH, h = SCREEN_HEIGHT;
	double time = 0, oldTime = 0;//, planeX = 0, planeY = 0.66;
	int quit = 0;
	//double buffer[screenHeight][screenWidth], texture[8][texWidth * texHeight];
	SDL_Window* window = initw();
	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);//, *tloader = NULL;
	struct playerpos player;
	struct bufarray buffer_array;

	player.posX = 21, player.posY = 12, player.dirX = -1, player.dirY = 0;
	player.planeX = 0, player.planeY = 0.66, player.time = 0, player.oldTime = 0;
	//texloader(texture, screenSurface);
	texloader(screenSurface);
	printf("Texture Error\n");
	while (quit == 0)
	{
		//player.posX = posX, player.posY = posY, player.dirX = dirX, player.dirY = dirY;
		//player.planeX = planeX, player.planeY = planeY, player.time = time, player.oldTime = oldTime;

		renderfloor(&player, &buffer_array);//, texture);
		//printf("Floor Error\n");

		for (int i = 0 ; i < screenHeight ; i++)
			for(int j = 0 ; j < screenWidth ; j++)
			{
				buffer[i][j] = (buffer_array.buf)[i][j];
				//printf("Floor %d %d:%f", i, j, buffer[i][j]);
			}

		/*
		//Floor rendering
		for(int y = 0; y < h; y++)
		{
			// rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
			float rayDirX0 = dirX - planeX;
			float rayDirY0 = dirY - planeY;
			float rayDirX1 = dirX + planeX;
			float rayDirY1 = dirY + planeY;

		// Current y position compared to the center of the screen (the horizon)
			int p = y - screenHeight / 2;

			// Vertical position of the camera.
			float posZ = 0.5 * screenHeight;

		// Horizontal distance from the camera to the floor for the current row.
		//0.5 is the z position exactly in the middle between floor and ceiling.
			float rowDistance = posZ / p;
			// calculate the real world step vector we 
			// have to add for each x (parallel to camera plane)
			// adding step by step avoids multiplications 
			// with a weight in the inner loop
			float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
			float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;
			// real world coordinates of the leftmost column.
			// This will be updated as we step to the right.
			float floorX = posX + rowDistance * rayDirX0;
			float floorY = posY + rowDistance * rayDirY0;

			for(int x = 0; x < screenWidth; ++x)
			{
				// the cell coord is simply got from the 
				// integer parts of floorX and floorY
				int cellX = (int)(floorX);
				int cellY = (int)(floorY);

				// get the texture coordinate from the fractional part
				int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
				int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

				floorX += floorStepX;
				floorY += floorStepY;

				// choose texture and draw the pixel
				int floorTexture = 3;
				int ceilingTexture = 6;
				Uint32 color;

				// floor
				color = texture[floorTexture][texWidth * ty + tx];
				color = (color >> 1) & 8355711; // make a bit darker
				buffer[y][x] = color;

				//ceiling (symmetrical, at screenHeight - y - 1 instead of y)
				color = texture[ceilingTexture][texWidth * ty + tx];
				color = (color >> 1) & 8355711; // make a bit darker
				buffer[screenHeight - y - 1][x] = color;
			}
		}
	*/
	
	
	
	/*Wall rendering
	//generate some textures
	for(int x = 0; x < texWidth; x++)
		for(int y = 0; y < texHeight; y++)
		{
			int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
			//int xcolor = x * 256 / texWidth;
			int ycolor = y * 256 / texHeight;
			int xycolor = y * 128 / texHeight + x * 128 / texWidth;
			texture[0][texWidth * y + x] = 65536 * 254 * (x != y && x != texWidth - y);
			//flat red texture with black cross
			texture[1][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor;
			//sloped greyscale
			texture[2][texWidth * y + x] = 256 * xycolor + 65536 * xycolor;
			//sloped yellow gradient
			texture[3][texWidth * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor;
			//xor greyscale
			texture[4][texWidth * y + x] = 256 * xorcolor;
			//xor green
			texture[5][texWidth * y + x] = 65536 * 192 * (x % 16 && y % 16);
			//red bricks
			texture[6][texWidth * y + x] = 65536 * ycolor;
			//red gradient
			texture[7][texWidth * y + x] = 128 + 256 * 128 + 65536 * 128;
			//flat grey texture
		}
	*/
	renderwall(&player);
/*	
	for(int x = 0; x < w; x++)
		{
			//calculate ray position and direction
			double cameraX = 2 * x / (double)w - 1;
			//x-coordinate in camera space
			double rayDirX = dirX + planeX * cameraX;
			double rayDirY = dirY + planeY * cameraX;
			//which box of the map we're in
			int mapX = (int)posX;
			int mapY = (int)posY;

			//length of ray from current position 
			//to next x or y-side
			double sideDistX;
			double sideDistY;

			//length of ray from one x or y-side to next x or y-side
			double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
			double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
			double perpWallDist;

			//to step in x or y-direction (either +1 or -1)
			int stepX;
			int stepY;

			int hit = 0; //was there a wall hit?
			int side; //was a NS or a EW wall hit?
			
			//calculate step and initial sideDist
			if (rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (posX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0 - posX) * deltaDistX;
			}
			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (posY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0 - posY) * deltaDistY;
			}
			//perform DDA
			while (hit == 0)
			{
				//jump to next map square, either in x-direction, or in y-direction
				if (sideDistX < sideDistY)
				{
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}
				//Check if ray has hit a wall
				if (worldMap[mapX][mapY] > 0) hit = 1;
			}
			//Calculate distance projected on camera
			//direction (Euclidean distance would give
			//fisheye effect!)
			if(side == 0) 
				perpWallDist = (sideDistX - deltaDistX);
			else
				perpWallDist = (sideDistY - deltaDistY);
			//Calculate height of line to draw on screen
			int lineHeight = (int)(h / perpWallDist);

			//calculate lowest and highest pixel to
			//fill in current stripe
			int drawStart = -lineHeight / 2 + h / 2;
			if(drawStart < 0)
				drawStart = 0;
			int drawEnd = lineHeight / 2 + h / 2;
			if(drawEnd >= h)
				drawEnd = h - 1;

			//texturing calculations
			int texNum = worldMap[mapX][mapY] - 1;
			//1 subtracted from it so that texture 0 can be used!
      
			//calculate value of wallX
			double wallX; //where exactly the wall was hit
			if (side == 0)
				wallX = posY + perpWallDist * rayDirY;
			else
				wallX = posX + perpWallDist * rayDirX;
			wallX -= floor(wallX);

			//x coordinate on the texture
			int texX = (int)(wallX * (double)texWidth);
			if(side == 0 && rayDirX > 0)
				texX = texWidth - texX - 1;
			if(side == 1 && rayDirY < 0)
				texX = texWidth - texX - 1;
			
			// How much to increase the texture coordinate per screen pixel
			double step = 1.0 * texHeight / lineHeight;
			// Starting texture coordinate
			double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
			for(int y = drawStart; y<drawEnd; y++)
			{
				// Cast the texture coordinate to integer, 
				// and mask with (texHeight - 1) in case of overflow
				int texY = (int)texPos & (texHeight - 1);
				texPos += step;
				Uint32 color = texture[texNum][texHeight * texY + texX];
				//make color darker for y-sides: R, G and B byte each 
				//divided through two with a "shift" and an "and"
				if(side == 1)
					color = (color >> 1) & 8355711;
				buffer[y][x] = color;
				//printf("SEG%d %d %f ", x, y, buffer[x][y]);
			}
		}
	*/
		unsigned int * pixels = screenSurface->pixels;
		int width = screenSurface->w;
		int height = screenSurface->h;
	
		/*printf("Render on w%d h%d!!!\n", width, height);
		printf("417%f\n\n", buffer[470][630]);*/
		// Set every pixel to white.
		for (int y = 0; y < height - 1; ++y)
		{
			for (int x = 0; x < width - 1; ++x)
			{
				//printf("Buffer %d %d %d ", x, y, (unsigned int)floor(buffer[y][x]));
				if (x > 640 || y > 640)
					pixels[x + y * width] = SDL_MapRGBA(screenSurface->format, 200, 50, 50, 255);
				else
					pixels[x + y * width] = (unsigned int)(buffer[y][x]);
				//SDL_MapRGBA(screenSurface->format, 200, 100, 250, 255);
			}
			//printf(" Line 2\n");
		}
		//printf("2!!!\n");
		SDL_UpdateWindowSurface(window);

		//drawBuffer(buffer[0]);
		for(int y = 0; y < h; y++)
			for(int x = 0; x < w; x++)
				buffer[y][x] = 0;
		//clear the buffer instead of cls()

		//timing for input and FPS counter
		oldTime = time;
		time = SDL_GetTicks();
		double frameTime = (time - oldTime) / 1000.0;
		//frameTime is the time this frame has taken, in seconds
		//printf("%f", 1.0 / frameTime); //FPS counter
		//SDL_Redraw();

		//speed modifiers
		double moveSpeed = frameTime * 3.5;//5.0
		//the constant value is in squares/second
		double rotSpeed = frameTime * 4;//3.0
		//the constant value is in radians/second

		//Event handler
		//SDL_Event e;	
		//struct App app;	
		//Handle events on queue
		playermov(&player, moveSpeed, rotSpeed);
	}
}
		/*
		while (SDL_PollEvent(&e) != 0)
		{
			double oldDirX = 0;
			double oldPlaneX = 0; 
			//User requests quit
			if (e.type == SDL_QUIT)
				quit = 1;
			//user presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
					case SDLK_UP:
						//move forward if no wall in front of you
						//printf("UP\n");
						if(worldMap[(int)(player.posX + player.dirX * moveSpeed)]
								[(int)(player.posY)] == 0)
							player.posX += player.dirX * moveSpeed;
						if(worldMap[(int)(player.posX)][(int)(player.posY + player.dirY * moveSpeed)] == 0)
							player.posY += player.dirY * moveSpeed;
						break;
					case SDLK_DOWN:
						//move backwards if no wall behind you
						if (worldMap[(int)(player.posX - player.dirX * moveSpeed)][(int)(player.posY)] == 0)
							player.posX -= player.dirX * moveSpeed;
						if (worldMap[(int)player.posX][(int)(player.posY - player.dirY * moveSpeed)] == 0)
							player.posY -= player.dirY * moveSpeed;
						break;
					case SDLK_RIGHT:
						//rotate to the right
						//camera direction and camera plane must be rotated
						oldDirX = player.dirX;
						player.dirX = player.dirX * cos(-rotSpeed) - player.dirY * sin(-rotSpeed);
						player.dirY = oldDirX * sin(-rotSpeed) + player.dirY * cos(-rotSpeed);
						oldPlaneX = player.planeX;
						player.planeX = player.planeX * cos(-rotSpeed) - player.planeY * sin(-rotSpeed);
						player.planeY = oldPlaneX * sin(-rotSpeed) + player.planeY * cos(-rotSpeed);
						break;
					case SDLK_LEFT:
						//rotate to the left
						//both camera direction and camera plane must be rotate
						oldDirX = player.dirX;
						player.dirX = player.dirX * cos(rotSpeed) - player.dirY * sin(rotSpeed);
						player.dirY = oldDirX * sin(rotSpeed) + player.dirY * cos(rotSpeed);
						oldPlaneX = player.planeX;
						player.planeX = player.planeX * cos(rotSpeed) - player.planeY * sin(rotSpeed);
						player.planeY = oldPlaneX * sin(rotSpeed) + player.planeY * cos(rotSpeed);
						break;
					case SDLK_UP && SDLK_LEFT:
						oldDirX = dirX;
                                                dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
                                                dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
                                                oldPlaneX = planeX;
                                                planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
                                                planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
printf("UP\n");
                                                if(worldMap[(int)(posX + dirX * moveSpeed)]
                                                                [(int)(posY)] == 0)
                                                        posX += dirX * moveSpeed;
                                                if(worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == 0)
                                                        posY += dirY * moveSpeed;
oldDirX = dirX;

						break;

				}
			}
			
		//doInput();

		if (app.up)
		{
			//move forward if no wall in front of you
			//printf("UP\n");
			if(worldMap[(int)(player.posX + player.dirX * moveSpeed
						)][(int)(player.posY)] == 0)
				player.posX += player.dirX * moveSpeed;
			if(worldMap[(int)(player.posX)][(int)(player.posY
						+ player.dirY * moveSpeed)] == 0)
				player.posY += player.dirY * moveSpeed;
		}

		if (app.down)
		{
			//move backwards if no wall behind you
			if (worldMap[(int)(player.posX - player.dirX
						* moveSpeed)][(int)(player.posY)] == 0)
				player.posX -= player.dirX * moveSpeed;
			if (worldMap[(int)player.posX][(int)(player.posY
						- player.dirY * moveSpeed)] == 0)
				player.posY -= player.dirY * moveSpeed;
		}

		if (app.left)
		{
			//rotate to the right
			//camera direction and camera plane must be rotated
			oldDirX = player.dirX;
			player.dirX = player.dirX * cos(-rotSpeed) - player.dirY * sin(-rotSpeed);
			player.dirY = oldDirX * sin(-rotSpeed) + player.dirY * cos(-rotSpeed);
			oldPlaneX = player.planeX;
			player.planeX = player.planeX * cos(-rotSpeed)
				- player.planeY * sin(-rotSpeed);
			player.planeY = oldPlaneX * sin(-rotSpeed) + player.planeY
				* cos(-rotSpeed);
		}

		if (app.right)
		{
			//rotate to the left
			//both camera direction and camera plane must be rotate
			oldDirX = player.dirX;
			player.dirX = player.dirX * cos(rotSpeed) - player.dirY
				* sin(rotSpeed);
			player.dirY = oldDirX * sin(rotSpeed) + player.dirY * cos(rotSpeed);
			oldPlaneX = player.planeX;
			player.planeX = player.planeX * cos(rotSpeed)
				- player.planeY * sin(rotSpeed);
			player.planeY = oldPlaneX * sin(rotSpeed) + player.planeY
				* cos(rotSpeed);
		}

		blit(player.texture, player.x, player.y);

		presentScene();

		SDL_Delay(16);
		}
	
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
			quit = 1;
		}
	}
}*/

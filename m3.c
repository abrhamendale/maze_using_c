#include "maze.h"
#include <SDL2/SDL_image.h>
#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480
#define texWidth 64
#define texHeight 64

void loader(double *p1, unsigned int *p2, unsigned int w, unsigned int h)
{
	//printf("Loader function ...!\n");
	printf("width:%uheight:%u\n", w, h);
	//for (int y = 0; y < height - 1; ++y)
	for (unsigned int x = 0 ; x < w ; ++x)
		for (unsigned int y = 0 ; y < h ; ++y)
		{
			//printf("Buffer %d %d %d ", x, y, (
			//unsigned int)floor(buffer[y][x]));
			p1[x + w * y] = (unsigned int)(p2[w * y + x]);
			//SDL_MapRGBA(screenSurface->format, 200, 100, 250, 255);
		}
}

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


int main()//int argc, char* args[])
{
	double posX = 22, posY = 12;  //x and y start position
	double w = 640, h = 480;
	double dirX = -1, dirY = 0; //initial direction vector
	double planeX = 0, planeY = 0.66; //the 2d version of camera plane
	double time = 0; //time of current frame
	double oldTime = 0; //time of previous frame
	int quit = 0;

	double buffer[screenHeight][screenWidth];
	// y-coordinate first because it works per scanline
	double texture[8][texWidth * texHeight];
	//for(int i = 0; i < 8; i++)
	//texture[i].resize(texWidth * texHeight);
	
	//unsigned long tw, th, error = 0;
	
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL, *tloader = NULL;
	//SDL_Texture* gtexture = NULL;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("SDL Tutorial!"
                                , SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
                                , SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	//printf("seg!!!\n");
	tloader = IMG_Load("pics/eagle.png");
	//printf("%d\n", tloader->w);
	loader(texture[0], tloader->pixels, (unsigned int)tloader->w, (unsigned int)tloader->h);
	printf("SEG!!!\n");
	tloader = IMG_Load("pics/redbrick.png");
	loader(texture[1], tloader->pixels, tloader->w, tloader->h);
	
	//for (int y = 0; y < 64 - 1; ++y)
	//{
	for (int x = 0; x < 4096; ++x)
	{
		//if (x > 640 || y > 640)
		//	((unsigned int *)(screenSurface->pixels))[x + y * (screenSurface->w)] = SDL_MapRGBA(screenSurface->format, 200, 50, 50, 255);
		//else
		((unsigned int *)(screenSurface->pixels))[x] = (unsigned int)(texture[1][x]);
	}
	//}

	
	while (quit == 0)
	{
		//SDL_BlitSurface(tloader, NULL, screenSurface, NULL);
		SDL_UpdateWindowSurface(window);
	}
	tloader = IMG_Load("pics/purplestone.png");
	loader(texture[2], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/greystone.png");
	loader(texture[3], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/bluestone.png");
	loader(texture[4], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/mossy.png");
	loader(texture[5], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/wood.png");
	loader(texture[6], tloader->pixels, tloader->w, tloader->h);
	tloader = IMG_Load("pics/colorstone.png");
	loader(texture[7], tloader->pixels, tloader->w, tloader->h);
/*
	error |= IMG_Load(texture[0], tw, th, "pics/eagle.png");
	error |= loadImage(texture[1], tw, th, "pics/redbrick.png");
	error |= loadImage(texture[2], tw, th, "pics/purplestone.png");
	error |= loadImage(texture[3], tw, th, "pics/greystone.png");
	error |= loadImage(texture[4], tw, th, "pics/bluestone.png");
	error |= loadImage(texture[5], tw, th, "pics/mossy.png");
	error |= loadImage(texture[6], tw, th, "pics/wood.png");
	error |= loadImage(texture[7], tw, th, "pics/colorstone.png");
	if(error)
	{
		printf("error loading images\n");
		return 1;
	}
*/

	while (quit == 0)
	{
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
		printf("2!!!\n");
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
		printf("%f", 1.0 / frameTime); //FPS counter
		//SDL_Redraw();

		//speed modifiers
		double moveSpeed = frameTime * 2;//5.0
		//the constant value is in squares/second
		double rotSpeed = frameTime * 3;//3.0
		//the constant value is in radians/second

		//Event handler
		SDL_Event e;	
		
		//Handle events on queue
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
						printf("UP\n");
						if(worldMap[(int)(posX + dirX * moveSpeed)]
								[(int)(posY)] == 0)
							posX += dirX * moveSpeed;
						if(worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == 0)
							posY += dirY * moveSpeed;
						break;
					case SDLK_DOWN:
						//move backwards if no wall behind you
						if (worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)] == 0)
							posX -= dirX * moveSpeed;
						if (worldMap[(int)posX][(int)(posY - dirY * moveSpeed)] == 0)
							posY -= dirY * moveSpeed;
						break;
					case SDLK_RIGHT:
						//rotate to the right
						//camera direction and camera plane must be rotated
						oldDirX = dirX;
						dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
						dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
						oldPlaneX = planeX;
						planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
						planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
						break;
					case SDLK_LEFT:
						//rotate to the left
						//both camera direction and camera plane must be rotate
						oldDirX = dirX;
						dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
						dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
						oldPlaneX = planeX;
						planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
						planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
				break;
				}
			}
		}
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
			quit = 1;
		}
	}
}
	//Destroy window

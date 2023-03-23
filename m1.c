#include "maze.h"
int main()//int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;        
	//            //The surface contained by the window
	SDL_Surface* screenSurface = NULL;

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

			//Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xAA, 0x44, 0x22));
                                           
			//Update the surface
			SDL_UpdateWindowSurface( window );
		        //Hack to get window to stay 
			SDL_Event e; 
			int quit = 0;
			while(quit == 0)
			{
				while(SDL_PollEvent(&e))
				{
					if(e.type == SDL_QUIT)
						quit = 1;
				}
			}
		}    
	}
	//Destroy window
	SDL_DestroyWindow(window);
	//Quit SDL subsystems
	SDL_Quit();
	return 0;
}

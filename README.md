## Project Name
Maze

![Screenshot](https://github.com/abrhamendale/maze_using_c/pics/eagle.png)
## Introduction
Maze is a game written in c. It is a simple 3D game made using raycasting algorithm. The library used is SDL2 and SDL2_Image. Programming a game in and SDL API has been a bit challenging. The main problem I encountered after I started coding was using SDL_image library. The library gets installed but during compilation errors appear that show as if it was not installed. Meaning the header files appear not to be installed after some searching, I found the problem had to do with how the code should be compiled. So, I added the necessary keywords and now my codes are compiling without errors. I have encountered the same issue for compiling with SDL header files. Both needed appropriate keywords to be used during compilation with gcc to work. Once the compilation problems were resolved what proved difficult was working with SDL_surface struct. This is a structure which contains the data to be drawn to the screen and comprises of among other things the main pixel information in a form of void * array, the width of the pixels and the height of the pixels to be drawn. Since the pixels data is in a single void *array it was difficult to find a way to render a 2-dimensional array to it. In addition to that it always needs to be converted to the appropriate data type to be used. (i.e., double *, unsigned int *, etc.)
## Installation
Since the game is written in c it can be compiled and used on any platform but gcc and the SDL libraries are required.
## Usage
The game needs to be compiled using the following options:
***gcc -Wall -Wextra -Werror -pedantic -I/usr/include/SDL/ -o Maze raycaster.c keylistener.c movement.c loaders.c launcher.c -L/usr/lib -lSDL2 -lm -lSDL2_image***
## Contributing
Me
## Related projects
The first of many
## Licensing
None

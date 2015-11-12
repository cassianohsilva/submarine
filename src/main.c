/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
 and may not be redistributed without written permission.*/

//Using SDL and standard IO
/*
 #include <stdio.h>
 #include <stdlib.h>
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>
 */
#include "player.h"
#include "resources.h"

typedef enum {
	true = 1, false = 0
} bool;

int HORIZONTAL_KEY_PRESSED = 0;
int VERTICAL_KEY_PRESSED = 0;

#define PASSO 10

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO) < 0 || SDL_Init( SDL_INIT_EVENTS) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	} else {
		atexit(SDL_Quit);

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL_Error: %s\n",
					SDL_GetError());
			success = false;
		} else {
			int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

			if (!(IMG_Init(imgFlags) & imgFlags)) {
				printf("SDL_image could not initialize! SDL_image Error: %s\n",
				IMG_GetError());
				success = false;
			} else {
				//Get window surface
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
	}

	return success;
}

bool loadMedia() {
	//Loading success flag
	bool success = true;

	//Load splash image
	/*
	 gXOut = SDL_LoadBMP("res/hello_world.bmp");
	 if (gXOut == NULL) {
	 printf("Unable to load image %s! SDL Error: %s\n",
	 "res/hello_world.bmp", SDL_GetError());
	 success = false;
	 }
	 */

	return success;
}

void close() {
	//Deallocate surface
	SDL_FreeSurface(gXOut);
	gXOut = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	//SDL_Quit();
}

int main(int argc, char* args[]) {
	//Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		//Load media
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Player * player = Player_create("res/hello_world.bmp");
			Player * player = Player_create(gWindow, RES_SUBMARINE);
			//Player * player = Player_create(gWindow, "res/submarino.png");

			//While application is running
			while (!quit) {

				//Handle events on queue
				//while (SDL_PollEvent(&e) != 0) {
				if (SDL_PollEvent(&e) != 0) {
					//User requests quit
					if (e.type == SDL_QUIT) {
						quit = true;
					} else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
							case SDLK_UP:
								// Para cima.
								VERTICAL_KEY_PRESSED = -1;
								break;
							case SDLK_DOWN:
								// Para baixo.
								VERTICAL_KEY_PRESSED = 1;
								break;
							case SDLK_LEFT:
								// Para a esquerda.
								HORIZONTAL_KEY_PRESSED = -1;
								break;
							case SDLK_RIGHT:
								// Para a direita.
								HORIZONTAL_KEY_PRESSED = 1;
								break;
						}

						Player_move(player, PASSO * HORIZONTAL_KEY_PRESSED, PASSO * VERTICAL_KEY_PRESSED, gScreenSurface->w,
								gScreenSurface->h);
					} else if (e.type == SDL_KEYUP) {
						switch (e.key.keysym.sym) {
						case SDLK_UP:
						case SDLK_DOWN:
							VERTICAL_KEY_PRESSED = 0;
							break;
						case SDLK_LEFT:
						case SDLK_RIGHT:
							HORIZONTAL_KEY_PRESSED = 0;
							break;
						}
					}
				}
				SDL_FillRect(gScreenSurface, NULL,
						SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));

				Player_render(player, gScreenSurface);
				/*
				 //Apply the image
				 SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
				 */
				//Update the surface
				SDL_UpdateWindowSurface(gWindow);

			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}


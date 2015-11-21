#include <time.h>

#include "player.h"
#include "enemy.h"
#include "game.h"
#include "resources.h"

int HORIZONTAL_KEY_PRESSED = 0;
int VERTICAL_KEY_PRESSED = 0;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

void close();

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gXOut = NULL;

bool init() {

	srand(time(NULL));

	bool success = true;

	if (SDL_Init( SDL_INIT_VIDEO) < 0 || SDL_Init( SDL_INIT_EVENTS) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	} else {
		atexit(SDL_Quit);

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
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
	}

	return success;
}

void control_player(Player * player, const Uint8 *keystates) {
	if (keystates[SDL_SCANCODE_UP]) {
		VERTICAL_KEY_PRESSED = -1;
	} else if (keystates[SDL_SCANCODE_DOWN]) {
		VERTICAL_KEY_PRESSED = 1;
	} else {
		VERTICAL_KEY_PRESSED = 0;
	}

	if (keystates[SDL_SCANCODE_LEFT]) {
		HORIZONTAL_KEY_PRESSED = -1;
	} else if (keystates[SDL_SCANCODE_RIGHT]) {
		HORIZONTAL_KEY_PRESSED = 1;
	} else {
		HORIZONTAL_KEY_PRESSED = 0;
	}

	if (keystates[SDL_SCANCODE_SPACE]) {
		Player_shot(player);
	}

	Player_move(player, HORIZONTAL_KEY_PRESSED, VERTICAL_KEY_PRESSED,
			gScreenSurface->w, gScreenSurface->h);
}

void close() {
	SDL_FreeSurface(gXOut);
	gXOut = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
}

int main(int argc, char* args[]) {
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		bool quit = false;

		SDL_Event e;

		Game * game = Game_create(gWindow);
		Player * player = game->player;

		const Uint8 *keystates = SDL_GetKeyboardState( NULL);

		while (!quit) {

			if (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}

			control_player(player, keystates);

			Game_update(game);
		}
	}

	close();

	return 0;
}


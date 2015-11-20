#include "player.h"
#include "enemy.h"
#include "game.h"
#include "resources.h"

int HORIZONTAL_KEY_PRESSED = 0;
int VERTICAL_KEY_PRESSED = 0;

#define PASSO 10

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

void close();

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gXOut = NULL;

bool init() {
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

void move_player(SDL_Event * e, Player * player) {
	if (e->type == SDL_KEYDOWN) {
		switch (e->key.keysym.sym) {
			case SDLK_UP:
				// Para cima
				VERTICAL_KEY_PRESSED = -1;
				break;
			case SDLK_DOWN:
				// Para baixo
				VERTICAL_KEY_PRESSED = 1;
				break;
			case SDLK_LEFT:
				// Para a esquerda
				HORIZONTAL_KEY_PRESSED = -1;
				break;
			case SDLK_RIGHT:
				// Para a direita
				HORIZONTAL_KEY_PRESSED = 1;
				break;
		}

		Player_move(player, PASSO * HORIZONTAL_KEY_PRESSED,
		PASSO * VERTICAL_KEY_PRESSED, gScreenSurface->w, gScreenSurface->h);
	} else if (e->type == SDL_KEYUP) {
		switch (e->key.keysym.sym) {
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

		Game * game = Game_create(10, gWindow);
		Player * player = game->player;

		while (!quit) {

			if (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
					move_player(&e, player);
				}
			}

			Game_update(game);
		}
	}

	close();

	return 0;
}


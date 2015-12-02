#include "player.h"
#include "enemy.h"
#include "game.h"
#include "resources.h"

int HORIZONTAL_KEY_PRESSED = 0;
int VERTICAL_KEY_PRESSED = 0;

//int SPAWNED = 0;

Game * game = NULL;

bool init();

void close_all();

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gXOut = NULL;

bool init() {

	bool success = true;

	if (SDL_Init( SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		fflush(stdout);
		success = false;
	} else {

		srand(time(NULL));

		gWindow = SDL_CreateWindow("Submarine", SDL_WINDOWPOS_UNDEFINED,
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

			if (TTF_Init() == -1) {
				printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
				TTF_GetError());
				success = false;
			}

			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
				printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
				Mix_GetError());
				success = false;
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
		Player_shot(player, game->bullets);
//		if(!SPAWNED) {
//			Game_spawn_enemy(game, SUBMARINE, LEFT, rand()%SCREEN_HEIGHT, 1.0);
//			SPAWNED = 1;
//		}
	}

	Player_move(player, HORIZONTAL_KEY_PRESSED, VERTICAL_KEY_PRESSED,
			gScreenSurface->w, gScreenSurface->h);
}

void close_all() {
	SDL_FreeSurface(gXOut);
	gXOut = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[]) {
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		bool quit = false;

		SDL_Event e;

		game = Game_create(gWindow);
		Player * player = game->player;

		const Uint8 *keystates = SDL_GetKeyboardState( NULL);

		while (!quit) {

			if (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_KEYUP) {
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						game->is_paused = !game->is_paused;

						if (!game->is_paused) {
							if (Timer_is_started(game->timer)) {
								Timer_stop(game->timer);
							} else {
								Timer_start(game->timer);
							}
						} else {
							if (Timer_is_paused(game->timer)) {
								Timer_unpause(game->timer);
							} else {
								Timer_pause(game->timer);
							}
						}
					}
				}
			}

			if (!game->is_paused) {
				control_player(player, keystates);

				float probability = ((float) rand()) / INT32_MAX;

				if (probability < 0.01) {
					EnemyType enemy_type =
							(rand() > (INT32_MAX >> 1)) ? SUBMARINE : SHARK;
					Direction direction =
							(rand() > (INT32_MAX >> 1)) ? RIGHT : LEFT;

				Game_spawn_enemy(game, enemy_type, direction,
						rand() % SCREEN_HEIGHT, 1.0);
			}

			probability = ((float) rand()) / INT32_MAX;

			if (probability < 0.005) {
				Direction direction =
						(rand() > (INT32_MAX >> 1)) ? RIGHT : LEFT;

			Game_spawn_diver(game, direction, rand() % SCREEN_HEIGHT,
					1.0);
		}
	}

	Game_update(game);
	SDL_Delay(5);
}
}

close_all();

return 0;
}

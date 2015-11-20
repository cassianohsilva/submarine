/*
 * game.c
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "game.h"

Game * Game_create(int max_enemies, SDL_Window * window) {
	Game * game = (Game *) malloc(sizeof(Game));

	if (game != NULL) {
		game->player = Player_create(window, RES_SUBMARINE);
		game->max_enemy = max_enemies;
		game->enemies = (Enemy **) malloc(max_enemies * sizeof(Enemy *));
		game->window = window;
		game->surface = SDL_GetWindowSurface(window);
	}

	return game;
}

void Game_update(Game * game) {

	Player_render(game->player, game->surface);

	int i;

	for(i = 0; i < game->max_enemy; i++) {

		Enemy * enemy = game->enemies[i];

		if(enemy != NULL) {
			Enemy_render(enemy, game->surface);
		}
	}

	SDL_FillRect(game->surface, NULL,
						SDL_MapRGB(game->surface->format, 0xFF, 0xFF, 0xFF));

	SDL_UpdateWindowSurface(game->window);
}

void Game_destroy(Game * game) {

	if (game != NULL) {
		int i = 0;

		Player_destroy(game->player);

		for (i = 0; i < game->max_enemy; i++) {
			Enemy_destroy(game->enemies[i]);
		}

		free(game->enemies);
		free(game);
	}
}

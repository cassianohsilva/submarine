/*
 * game.c
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "game.h"

Game * Game_create(SDL_Window * window) {
	Game * game = (Game *) malloc(sizeof(Game));

	if (game != NULL) {
		game->player = Player_create(window, RES_SUBMARINE);
		game->enemies = List_create();
		game->window = window;
		game->surface = SDL_GetWindowSurface(window);
	}

	return game;
}

Enemy * Game_spawn_enemy(Game * game, EnemyType type, Direction direction,
		int y, float velocity_factor) {

	Enemy * enemy = NULL;

	if (type == SHARK) {
		enemy = Enemy_create(game->window, RES_SHARK, type, direction, y,
				velocity_factor);
	} else if (type == SUBMARINE) {
		// TODO Adicionar a imagem do submarino
		//enemy = Enemy_create(game->window, RES_SHARK, type);
	}

	if (enemy != NULL) {
		List_insert(game->enemies, (void *) enemy);
	}

	return enemy;
}

void Game_destroy_enemy(Game * game, Enemy * enemy) {
	List_remove(game->enemies, (void *) enemy);
	Enemy_destroy(enemy);
}

void Game_update(Game * game) {

	SDL_FillRect(game->surface, NULL,
			SDL_MapRGB(game->surface->format, 0xFF, 0xFF, 0xFF));

	Player_render(game->player, game->surface);

	Node * actual = game->enemies->begin;

	while (actual != NULL) {
		if (actual->value != NULL) {
			Node * prox = actual->next;
			Enemy * enemy = (Enemy *) actual->value;

			actual = prox;

			Enemy_move(enemy);

			if (Enemy_is_visible(enemy)) {
				Enemy_render(enemy, game->surface);
			} else {
				Game_destroy_enemy(game, enemy);
			}
		} else {
			actual = actual->next;
		}
	}

	SDL_UpdateWindowSurface(game->window);
}

void Game_destroy(Game * game) {

	if (game != NULL) {

		Player_destroy(game->player);

		Node * actual = game->enemies->begin;

		while (actual != NULL) {

			if (actual->value != NULL) {
				Enemy_destroy((Enemy *) game->enemies);
			}

			actual = actual->next;
		}

		List_destroy(game->enemies);
		free(game);
	}
}

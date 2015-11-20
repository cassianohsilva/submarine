/*
 * game.h
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "types.h"
#include "player.h"
#include "enemy.h"

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

typedef struct {
	Player * player;
	int max_enemy;
	Enemy ** enemies;

	SDL_Window * window;
	SDL_Surface * surface;
} Game;

Game * Game_create(int max_enemies, SDL_Window * window);

void Game_destroy(Game * game);

void Game_update(Game * game);

#endif /* SRC_GAME_H_ */

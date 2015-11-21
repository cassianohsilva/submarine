/*
 * game.h
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "types.h"
#include "player.h"
#include "enemy.h"
#include "resources.h"
#include "linked_list.h"

#define MOVEMENT_FACTOR 1.0
#define TIME_BETWEEN_SHOTS 50

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

typedef struct {
	Player * player;
	List * enemies;
	List * bullets;

	SDL_Window * window;
	SDL_Surface * surface;
} Game;

Game * Game_create(SDL_Window * window);

void Game_destroy(Game * game);

Enemy * Game_spawn_enemy(Game * game, EnemyType type, Direction direction, int y, float velocity_factor);

void Game_destroy_enemy(Game * game, Enemy * enemy);

void Game_update(Game * game);

void Game_update_bullets(Game * game);

#endif /* SRC_GAME_H_ */

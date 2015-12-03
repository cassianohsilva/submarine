/*
 * game.h
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "types.h"
#include "player.h"
#include "diver.h"
#include "enemy.h"
#include "timer.h"
#include "resources.h"
#include "linked_list.h"
#include "oxygen_bar.h"

#define MOVEMENT_FACTOR 1.0
#define TIME_BETWEEN_SHOTS 50
#define MAX_ENEMIES_ON_SCREEN 10
#define MAX_DIVERS_ON_SCREEN 3

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

typedef struct {
	Player * player;
	List * enemies;
	Uint8 enemies_on_screen;
	Uint8 divers_on_screen;
	List * bullets;
	List * divers;
	SDL_Rect breathe_zone;
	bool is_paused;
	Uint8 spawn_zone_size;
	Timer * timer;
	OxygenBar * oxygen_bar;

	SDL_Color score_color;
	TTF_Font * font;
	SDL_Rect * score_rect;
	SDL_Surface * score_surface;

	Mix_Chunk * explosion_sound;
	Mix_Chunk * rescue_sound;

	SDL_Window * window;
	SDL_Surface * surface;
} Game;

Game * Game_create(SDL_Window * window);

bool Game_is_player_breathing(Game * game);

Enemy * Game_spawn_enemy(Game * game);

Diver * Game_spawn_diver(Game * game, Direction direction, int y, float movement_factor);

void Game_destroy_enemy(Game * game, Enemy * enemy);

void Game_destroy_diver(Game * game, Diver * diver);

void Game_destroy_bullet(Game * game, Bullet * bullet);

void Game_update(Game * game);

void Game_update_score_surface(Game * game);

void Game_update_enemies(Game * game);

void Game_update_divers(Game * game);

void Game_destroy_divers(Game * game);

void Game_destroy_enemies(Game * game);

void Game_destroy_bullets(Game* game);

void Game_check_bullets_collision(Game * game);

void Game_check_divers_collision(Game * game);

void Game_update_bullets(Game * game);

void Game_destroy(Game * game);

#endif /* SRC_GAME_H_ */

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
#include "menu.h"
#include "input.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

typedef struct {
	bool is_locked;
	Uint8 enemies_number;
	Direction direction;
	EnemyType enemy_type;
} ZoneLock;

typedef struct {
	Player * player;
	Enemy * enemy_on_surface;
	List * enemies;
	Uint8 enemies_on_screen;
	Uint8 divers_on_screen;
	List * bullets;
	List * divers;
	SDL_Rect breathe_zone;
	bool is_started;
	bool is_paused;
	bool is_editing;
	Uint8 spawn_zone_size;
	Timer * timer;

	// Ground
	SDL_Rect ground_rect;
	OxygenBar * oxygen_bar;

	// Divers
	SDL_Surface * diver_icon;
	SDL_Rect diver_icon_rect;

	// Menus
	Menu * pause_menu;
	Menu * main_menu;

	Input * input;

	Mix_Music * background_music;

	ZoneLock * zone_lock;

	// Score
	SDL_Color score_color;
	TTF_Font * font;
	SDL_Rect * score_rect;
	SDL_Surface * score_surface;

	// Sounds
	Mix_Chunk * explosion_sound;
	Mix_Chunk * rescue_sound;

	SDL_Window * window;
	SDL_Surface * surface;
} Game;

Game * Game_create(SDL_Window * window);

bool Game_is_player_breathing(Game * game);

void Game_reset(Game * game);

void Game_start(Game * game);

void Game_show_input_name(Game * game, bool show);

void Game_stop(Game * game);

Enemy * Game_spawn_enemy(Game * game);

void Game_spawn_enemy_on_surface(Game * game);

Diver * Game_spawn_diver(Game * game);

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

void Game_check_enemies_collision(Game * game);

void Game_update_bullets(Game * game);

void Game_destroy(Game * game);

#endif /* SRC_GAME_H_ */

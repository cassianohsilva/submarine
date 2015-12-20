/*
 * player.c
 *
 *  Created on: 30 de out de 2015
 *      Author: cassiano
 */

#include "player.h"

Player * Player_create(SDL_Window * window, const char * filename, int x, int y,
		float movement_factor, Uint32 time_between_shots) {

	Player *player = (Player *) malloc(sizeof(Player));

	if (player != NULL) {
		player->window = window;
		player->surface = IMG_Load(filename);

		if (player->surface != NULL) {

			player->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
			player->sprite_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			player->rect->x = x;
			player->rect->y = y;

			player->rect->w = player->surface->w >> 1;
			player->rect->h = player->surface->h;

			player->look_dir = LEFT;

			player->sprite_rect->x = 0;
			player->sprite_rect->y = 0;
			player->sprite_rect->w = player->surface->w >> 1;
			player->sprite_rect->h = player->surface->h;

			player->oxygen = 100;
			player->divers_rescued = 0;
			player->score = 0;
			player->movement_factor = movement_factor;
			player->time_between_shots = time_between_shots;
			player->time_shot_counter = time_between_shots;
			player->collision_mask = PLAYER_LAYER;
		} else {
			printf("Erro ao carregar a imagem \'%s\': %s\n", filename,
			IMG_GetError());
		}
	}

	return player;
}

void Player_shot(Player* player, List * bullets) {
	if (player->time_shot_counter >= player->time_between_shots) {
		int x = (player->rect->x + (player->rect->x + player->rect->w)) >> 1;
		int y = (player->rect->y + (player->rect->y + player->rect->h)) >> 1;

		Bullet * bullet = Bullet_create(player->window, player->look_dir,
				player->movement_factor * 2, x, y, PLAYER_LAYER, RES_BULLET);

		List_insert(bullets, bullet);

		player->time_shot_counter = 0;
	}
}

void Player_render(Player * player, SDL_Surface * parent) {

	switch (player->look_dir) {

		case LEFT:
			player->sprite_rect->x = 0;
			player->sprite_rect->w = player->surface->w >> 1;
			break;
		case RIGHT:
			player->sprite_rect->x = player->surface->w >> 1;
			player->sprite_rect->w = player->surface->w;
			break;
	}

	if (player->time_shot_counter < player->time_between_shots) {
		player->time_shot_counter++;
	}

	SDL_BlitSurface(player->surface, player->sprite_rect, parent, player->rect);
}

void Player_set_position(Player * player, int x, int y) {
	if (player) {
		player->rect->x = x;
		player->rect->y = y;
	}
}

void Player_move(Player * player, int h, int v, int x_max, int y_min, int y_max) {
	if (player != NULL) {

		SDL_Rect * r = player->rect;

		int h_move = h * player->movement_factor;
		int v_move = v * player->movement_factor;

		short int to_right = h_move > 0;
		short int to_bottom = v_move > 0;

		if (h_move) {
			if (to_right) {

				int rest = x_max - (r->x + r->w);

				if (rest >= h_move) {
					r->x += h_move;
				} else {
					r->x += rest;
				}

				player->look_dir = RIGHT;

			} else {
				int rest = -r->x;

				if (rest <= h_move) {
					r->x += h_move;
				} else {
					r->x += rest;
				}

				player->look_dir = LEFT;
			}
		}

		if (v_move) {
			if (to_bottom) {

				int rest = y_max - (r->y + r->h);

				if (rest >= v_move) {
					r->y += v_move;
				} else {
					r->y += rest;
				}
			} else {
				int rest = y_min -r->y;

				if (rest <= v_move) {
					r->y += v_move;
				} else {
					r->y += rest;
				}
			}
		}
	}
}

void Player_destroy(Player * player) {
	free(player->rect);
	SDL_FreeSurface(player->surface);
	free(player);
}

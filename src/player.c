/*
 * player.c
 *
 *  Created on: 30 de out de 2015
 *      Author: cassiano
 */

#include <stdio.h>
#include "player.h"

Player * Player_create(SDL_Window * window, const char * filename) {

	Player *player = (Player *) malloc(sizeof(Player));

	if (player != NULL) {
		//player->surface = SDL_LoadBMP(filename);

		player->window = window;
		player->surface = IMG_Load(filename);

		if (player->surface != NULL) {

			player->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
			player->sprite_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			player->rect->x = 0;
			player->rect->y = 0;

			player->rect->w = (int) (player->surface->w * 0.5);
			player->rect->h = player->surface->h;

			player->look_dir = LEFT;

			player->sprite_rect->x = 0;
			player->sprite_rect->y = 0;
			player->sprite_rect->w = (int) (player->surface->w * 0.5);
			player->sprite_rect->h = player->surface->h;

			//player->sprite_rect = player->rect;

		} else {
			printf("Erro ao carregar a imagem: %s\n", IMG_GetError());
		}
	}

	return player;
}

void Player_render(const Player * player, SDL_Surface * parent) {

	switch (player->look_dir) {

		case LEFT:
			player->sprite_rect->x = 0;
			player->sprite_rect->w = (int) (player->surface->w * 0.5);
			break;
		case RIGHT:
			player->sprite_rect->x = (int) (player->surface->w * 0.5);
			player->sprite_rect->w = player->surface->w;
			break;
	}

	SDL_BlitSurface(player->surface, player->sprite_rect, parent, player->rect);
}

void Player_move(Player * player, int h_move, int v_move, int x_max, int y_max) {
	if (player != NULL) {

		SDL_Rect * r = player->rect;

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
				int rest = -r->y;

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
	free(player->surface);
	free(player);

	player = NULL;
}

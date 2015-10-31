/*
 * player.c
 *
 *  Created on: 30 de out de 2015
 *      Author: cassiano
 */

#include "player.h"

Player * Player_create(const char * filename) {

	Player *player = (Player *) malloc(sizeof(Player));

	if (player != NULL) {
		//player->surface = SDL_LoadBMP(filename);
		player->surface = IMG_Load(filename);

		if (player->surface != NULL) {

			player->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			player->rect->x = 0;
			player->rect->y = 0;
			player->rect->w = player->surface->w;
			player->rect->h = player->surface->h;
		} else {
			printf("Erro ao carregar a imagem: %s\n", IMG_GetError());
		}
	}

	return player;
}

void Player_render(const Player * player, SDL_Surface * parent) {
	SDL_BlitSurface(player->surface, NULL, parent, player->rect);
}

void Player_move(const Player * player, int h_move, int v_move, int x_max,
		int y_max) {
	if (player != NULL) {

		SDL_Rect * r = player->rect;

		/// TODO Fazer a verificação de movimento restante pare cima e para a esquerda
		if ((r->x + h_move) >= 0) {
			if ((r->x + r->w + h_move) <= x_max) {
				r->x += h_move;
			} else if ((r->x + r->w) < x_max) {
				r->x += x_max - (r->x + r->w);
			}
		}

		if ((r->y + v_move) >= 0) {
			if (((r->y + r->h + v_move) <= y_max)) {
				r->y += v_move;
			} else if (((r->y + r->h) < y_max)) {
				r->y += y_max - (r->y + r->h);
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

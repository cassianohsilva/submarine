/*
 * enemy.c
 *
 *  Created on: 12 de nov de 2015
 *      Author: cassiano
 */
#include "enemy.h"

Enemy * Enemy_create(SDL_Window * window, const char * filename) {

	Enemy *enemy = (Enemy *) malloc(sizeof(Enemy));

	if (enemy != NULL) {
		enemy->window = window;
		enemy->surface = IMG_Load(filename);

		if (enemy->surface != NULL) {

			enemy->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
			enemy->sprite_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			enemy->rect->x = 0;
			enemy->rect->y = 0;

			enemy->rect->w = (int) (enemy->surface->w * 0.5);
			enemy->rect->h = enemy->surface->h;

			enemy->look_dir = LEFT;

			enemy->sprite_rect->x = 0;
			enemy->sprite_rect->y = 0;
			enemy->sprite_rect->w = (int) (enemy->surface->w * 0.5);
			enemy->sprite_rect->h = enemy->surface->h;

			//enemy->sprite_rect = enemy->rect;

		} else {
			printf("Erro ao carregar a imagem: %s\n", IMG_GetError());
		}
	}

	return enemy;
}

void Enemy_render(const Enemy * enemy, SDL_Surface * parent) {
	switch (enemy->look_dir) {

		case LEFT:
			enemy->sprite_rect->x = 0;
			enemy->sprite_rect->w = (int) (enemy->surface->w * 0.5);
			break;
		case RIGHT:
			enemy->sprite_rect->x = (int) (enemy->surface->w * 0.5);
			enemy->sprite_rect->w = enemy->surface->w;
			break;
	}

	SDL_BlitSurface(enemy->surface, enemy->sprite_rect, parent, enemy->rect);
}

void Enemy_move(Enemy * enemy, int h_move, int v_move, int x_max, int y_max) {
	// TODO Implement this function
}

bool Enemy_is_visible(Enemy * enemy) {
	// TODO Implement this function
	return false;
}

void Enemy_destroy(Enemy * enemy) {
	free(enemy->rect);
	free(enemy->surface);
	free(enemy);

	enemy = NULL;
}

/*
 * enemy.c
 *
 *  Created on: 12 de nov de 2015
 *      Author: cassiano
 */
#include "enemy.h"

Enemy * Enemy_create(SDL_Window * window, const char * filename, EnemyType type) {

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

void Enemy_move(Enemy * enemy, int h_move, int v_move) {
	if (enemy != NULL) {
		SDL_Rect * r = enemy->rect;

		short int to_right = h_move > 0;

		if (h_move) {
			r->x += h_move;

			if (to_right) {
				enemy->look_dir = RIGHT;
			} else {
				enemy->look_dir = LEFT;
			}
		}

		if (v_move) {
			r->y += v_move;
		}
	}
}

bool Enemy_is_visible(Enemy * enemy) {
	SDL_Surface * surface = SDL_GetWindowSurface(enemy->window);
	bool is_visible = true;

	SDL_Rect * enemy_rect = enemy->rect;
	SDL_Rect screen_rect = { 0, 0, surface->w, surface->h };

	if (enemy_rect->x + enemy_rect->w < 0 || enemy_rect->x > screen_rect.w
			|| enemy_rect->y + enemy_rect->h < 0
			|| enemy_rect->y > screen_rect.h) {
		is_visible = false;
	}

	return is_visible;
}

void Enemy_destroy(Enemy * enemy) {
	free(enemy->rect);
	free(enemy->surface);
	free(enemy);
}

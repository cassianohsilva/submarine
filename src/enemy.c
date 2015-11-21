/*
 * enemy.c
 *
 *  Created on: 12 de nov de 2015
 *      Author: cassiano
 */
#include "enemy.h"

Enemy * Enemy_create(SDL_Window * window, const char * filename, EnemyType type,
		Direction direction, int y, float velocity_factor,
		int time_between_shots) {

	Enemy *enemy = (Enemy *) malloc(sizeof(Enemy));

	if (enemy != NULL) {
		enemy->window = window;
		enemy->surface = IMG_Load(filename);

		if (enemy->surface != NULL) {

			enemy->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
			enemy->sprite_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			enemy->rect->w = (int) (enemy->surface->w * 0.5);
			enemy->rect->h = enemy->surface->h;

			enemy->rect->y = y;

			if (direction == LEFT) {
				enemy->rect->x = SDL_GetWindowSurface(enemy->window)->w;
			} else {
				enemy->rect->x = 0;
			}

			enemy->direction = direction;
			enemy->movement_factor = velocity_factor;

			enemy->sprite_rect->y = 0;
			enemy->sprite_rect->w = (int) (enemy->surface->w * 0.5);
			enemy->sprite_rect->h = enemy->surface->h;

			if (direction == LEFT) {
				enemy->sprite_rect->x = 0;
			} else {
				enemy->sprite_rect->x = (int) (enemy->surface->w * 0.5);
			}

			enemy->type = type;

			if (type == SUBMARINE) {
				enemy->real_enemy = (EnemySubmarine *) malloc(
						sizeof(EnemySubmarine));

				EnemySubmarine * enemy_submarine =
						(EnemySubmarine *) enemy->real_enemy;

				enemy_submarine->bullet_list = List_create();
				enemy_submarine->time_between_shots = time_between_shots * 2;
				enemy_submarine->time_shot_counter =
						enemy_submarine->time_between_shots;
			}

		} else {
			printf("Erro ao carregar a imagem: %s\n", IMG_GetError());
		}
	}

	return enemy;
}

void Enemy_render(const Enemy * enemy, SDL_Surface * parent) {
	switch (enemy->direction) {

		case LEFT:
			enemy->sprite_rect->x = 0;
			enemy->sprite_rect->w = (int) (enemy->surface->w * 0.5);
			break;
		case RIGHT:
			enemy->sprite_rect->x = (int) (enemy->surface->w * 0.5);
			enemy->sprite_rect->w = enemy->surface->w;
			break;
	}

	if (enemy->type == SUBMARINE) {

		EnemySubmarine * submarine = (EnemySubmarine *) enemy->real_enemy;

		Node * node = submarine->bullet_list->begin;

		if (submarine->time_shot_counter < submarine->time_between_shots) {
			submarine->time_shot_counter++;
		}

		if (submarine->time_shot_counter >= submarine->time_between_shots) {

			float random = (rand() * 1.0) / INT32_MAX;

			if (random < 0.01) {

				int x = (enemy->rect->x + (enemy->rect->x + enemy->rect->w))
						>> 1;
				int y = (enemy->rect->y + (enemy->rect->y + enemy->rect->h))
						>> 1;

				Bullet * bullet = Bullet_create(enemy->window, enemy->direction,
						(enemy->movement_factor * 2.0), x, y, RES_BULLET);

				List_insert(submarine->bullet_list, bullet);

				submarine->time_shot_counter = 0;
			}
		}

		while (node != NULL) {
			Bullet * bullet = (Bullet *) node->value;

			node = node->next;

			Bullet_move(bullet);

			if (Bullet_is_visible(bullet)) {
				Bullet_render(bullet, parent);
			} else {
				List_remove(submarine->bullet_list, bullet);
				Bullet_destroy(bullet);
			}
		}
	}

	SDL_BlitSurface(enemy->surface, enemy->sprite_rect, parent, enemy->rect);
}

void Enemy_move(Enemy * enemy) {
	if (enemy != NULL) {
		enemy->rect->x += (int) (enemy->direction * enemy->movement_factor);
	}
}

bool Enemy_is_visible(Enemy * enemy) {
	SDL_Surface * surface = SDL_GetWindowSurface(enemy->window);
	bool is_visible = true;

	SDL_Rect * enemy_rect = enemy->rect;
	SDL_Rect screen_rect = { 0, 0, surface->w, surface->h };

	if (enemy_rect->x < 0 || enemy_rect->x > screen_rect.w || enemy_rect->y < 0
			|| enemy_rect->y > screen_rect.h) {
		is_visible = false;
	}

	return is_visible;
}

void Enemy_destroy(Enemy * enemy) {
	free(enemy->rect);
	SDL_FreeSurface(enemy->surface);
	free(enemy);
}

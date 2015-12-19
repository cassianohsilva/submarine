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

			enemy->rect->w = enemy->surface->w >> 1;
			enemy->rect->h = enemy->surface->h;

			enemy->rect->y = y;

			if (direction == LEFT) {
				enemy->rect->x = SDL_GetWindowSurface(enemy->window)->w;
			} else {
				enemy->rect->x = 0;
			}

			enemy->default_x = enemy->rect->x;
			enemy->aux_x = enemy->rect->x;

			if (enemy->rect->x >= 0
					&& enemy->rect->x
							<= SDL_GetWindowSurface(enemy->window)->w) {
				enemy->entred_on_screen = true;
			} else {
				enemy->entred_on_screen = false;
			}

			enemy->direction = direction;
			enemy->movement_factor = velocity_factor;

			enemy->sprite_rect->y = 0;
			enemy->sprite_rect->w = enemy->surface->w >> 1;
			enemy->sprite_rect->h = enemy->surface->h;

			if (direction == LEFT) {
				enemy->sprite_rect->x = 0;
			} else {
				enemy->sprite_rect->x = enemy->surface->w >> 1;
			}

			enemy->type = type;
			enemy->collision_mask = ENEMY_LAYER;

			if (type == SUBMARINE) {
				enemy->real_enemy = (EnemySubmarine *) malloc(
						sizeof(EnemySubmarine));

				EnemySubmarine * enemy_submarine =
						(EnemySubmarine *) enemy->real_enemy;

				enemy_submarine->time_between_shots = time_between_shots << 1;
				enemy_submarine->time_shot_counter =
						enemy_submarine->time_between_shots;
			} else {
				enemy->real_enemy = NULL;
			}

		} else {
			printf("Erro ao carregar a imagem: %s\n", IMG_GetError());
		}
	}

	return enemy;
}

void Enemy_render(const Enemy * enemy, SDL_Surface * parent, List * bullets) {
	switch (enemy->direction) {

		case LEFT:
			enemy->sprite_rect->x = 0;
			enemy->sprite_rect->w = enemy->surface->w >> 1;
			break;
		case RIGHT:
			enemy->sprite_rect->x = enemy->surface->w >> 1;
			enemy->sprite_rect->w = enemy->surface->w;
			break;
	}

	if (enemy->type == SUBMARINE) {

		EnemySubmarine * submarine = (EnemySubmarine *) enemy->real_enemy;

		if (submarine->time_shot_counter < submarine->time_between_shots) {
			submarine->time_shot_counter++;
		}

		if (submarine->time_shot_counter >= submarine->time_between_shots) {

			float random = ((float) rand()) / INT32_MAX;

			if (random < 0.01) {
				int x = (enemy->rect->x + (enemy->rect->x + enemy->rect->w))
						>> 1;
				int y = (enemy->rect->y + (enemy->rect->y + enemy->rect->h))
						>> 1;

				Bullet * bullet = Bullet_create(enemy->window, enemy->direction,
						(enemy->movement_factor * 2.0), x, y, ENEMY_LAYER,
						RES_ENEMY_BULLET);

				List_insert(bullets, bullet);

				submarine->time_shot_counter = 0;
			}
		}
	}

	SDL_BlitSurface(enemy->surface, enemy->sprite_rect, parent, enemy->rect);
}

void Enemy_move(Enemy * enemy) {
	if (enemy != NULL) {
		enemy->aux_x += (int) (enemy->direction * enemy->movement_factor);
		enemy->rect->x = enemy->aux_x;

		if (enemy->aux_x < 0) {
			enemy->sprite_rect->x = enemy->default_x - enemy->aux_x;
			enemy->sprite_rect->w = (enemy->surface->w >> 1) + enemy->aux_x;
		}
	}
}

bool Enemy_is_visible(Enemy * enemy) {
	SDL_Surface * surface = SDL_GetWindowSurface(enemy->window);
	bool is_visible = true;

	SDL_Rect * enemy_rect = enemy->rect;
	SDL_Rect screen_rect = { 0, 0, surface->w, surface->h };

	if (enemy_rect->x > screen_rect.w || enemy_rect->y < 0
			|| enemy_rect->y > screen_rect.h || (enemy->sprite_rect->w <= 0)) {

		is_visible = false;
	}

	return is_visible;
}

bool Enemy_is_entered_on_screen(Enemy * enemy) {
	if (enemy) {
		return enemy->entred_on_screen;
	}

	return false;
}

void Enemy_destroy(Enemy * enemy) {
	free(enemy->rect);

	if (enemy->real_enemy) {
		free(enemy->real_enemy);
	}
	SDL_FreeSurface(enemy->surface);
	free(enemy);
}

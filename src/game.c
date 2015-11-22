/*
 * game.c
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "game.h"

Game * Game_create(SDL_Window * window) {
	Game * game = (Game *) malloc(sizeof(Game));

	if (game != NULL) {
		game->player = Player_create(window, RES_SUBMARINE, MOVEMENT_FACTOR,
		TIME_BETWEEN_SHOTS);
		game->enemies = List_create();
		game->bullets = List_create();
		game->window = window;
		game->enemies_on_screen = 0;
		game->is_paused = false;
		game->surface = SDL_GetWindowSurface(window);

		SDL_Rect breathe_zone = { 0, 0, game->surface->w,
				(game->surface->h >> 3) };

		game->breathe_zone = breathe_zone;
		game->timer = Timer_create();
	}

	return game;
}

bool Game_is_player_breathing(Game * game) {

	SDL_Rect rect;
	SDL_bool b = SDL_IntersectRect(&game->breathe_zone, game->player->rect, &rect);

	return b? true: false;
}

bool collision_check(SDL_Rect * element_1, CollisionMask mask_1,
		SDL_Rect * element_2, CollisionMask mask_2) {

	bool is_colliding = false;

	if (mask_1 ^ mask_2) {

		SDL_Rect rect;
		SDL_bool b = SDL_IntersectRect(element_1, element_2, &rect);

		is_colliding = b ? true : false;
	}

	return is_colliding;
}

Enemy * Game_spawn_enemy(Game * game, EnemyType type, Direction direction,
		int y, float velocity_factor) {

	Enemy * enemy = NULL;

	if (game->enemies_on_screen < MAX_ENEMIES_ON_SCREEN) {
		if (type == SHARK) {
			enemy = Enemy_create(game->window, RES_SHARK, type, direction, y,
					velocity_factor, 0);
		} else if (type == SUBMARINE) {
			enemy = Enemy_create(game->window, RES_ENEMY_SUBMARINE, type,
					direction, y, velocity_factor, TIME_BETWEEN_SHOTS);
		}

		if (enemy != NULL) {
			List_insert(game->enemies, (void *) enemy);
			game->enemies_on_screen++;
		}
	}

	return enemy;
}

void Game_destroy_enemy(Game * game, Enemy * enemy) {
	List_remove(game->enemies, (void *) enemy);
	Enemy_destroy(enemy);

	game->enemies_on_screen--;
}

void Game_destroy_bullet(Game * game, Bullet * bullet) {
	List_remove(game->bullets, (void *) bullet);
	Bullet_destroy(bullet);
}

void Game_update(Game * game) {

	SDL_FillRect(game->surface, NULL,
			SDL_MapRGB(game->surface->format, 0x00, 0x66, 0xFF));

	Player_render(game->player, game->surface);

	if(!Game_is_player_breathing(game)) {
		if(game->player->oxygen) {
			game->player->oxygen--;
		}
	} else {
		if(game->player->oxygen < 100) {
			game->player->oxygen += 2;
		}
	}

//	printf("Player oxygen: %d\n", game->player->oxygen);

	Game_update_enemies(game);
	Game_update_bullets(game);

	Game_check_bullets_collision(game);

	SDL_UpdateWindowSurface(game->window);
}

void Game_update_enemies(Game * game) {
	Node * actual = game->enemies->begin;

	while (actual != NULL) {
		if (actual->value != NULL) {
			Node * prox = actual->next;
			Enemy * enemy = (Enemy *) actual->value;

			actual = prox;

			Enemy_move(enemy);

			if (Enemy_is_visible(enemy)) {
				Enemy_render(enemy, game->surface, game->bullets);
			} else {
				Game_destroy_enemy(game, enemy);
			}
		} else {
			actual = actual->next;
		}
	}
}

void Game_update_bullets(Game * game) {
	Node * node = game->bullets->begin;

	while (node != NULL) {

		Bullet * bullet = (Bullet *) node->value;

		node = node->next;

		Bullet_move(bullet);

		if (Bullet_is_visible(bullet)) {
			Bullet_render(bullet, game->surface);
		} else {
			List_remove(game->bullets, bullet);
			Bullet_destroy(bullet);
		}
	}
}

void Game_check_bullets_collision(Game * game) {

	Node * node = game->bullets->begin;
	Node * aux = NULL;

	while (node != NULL) {
		aux = node->next;

		Bullet * bullet = (Bullet *) node->value;

		Node * node_enemy = game->enemies->begin;
		Node * aux_enemy = NULL;

		while (node_enemy != NULL) {
			aux_enemy = node_enemy->next;

			Enemy * enemy = (Enemy *) node_enemy->value;

			bool collision = collision_check(bullet->rect,
					bullet->collision_mask, enemy->rect, enemy->collision_mask);

			if (collision) {
				Game_destroy_enemy(game, enemy);
				Game_destroy_bullet(game, bullet);
			}

			node_enemy = aux_enemy;
		}

		// TODO Adicionar verificação de colisão entre o jogador e os tiros

//		bool collision = collision_check(bullet->rect, bullet->collision_mask,
//				game->player->rect, game->player->collision_mask);

//		if (collision) {
//			Enemy_destroy(enemy);
//			Bullet_destroy(bullet);
//		}

		node = aux;
	}
}

void Game_destroy(Game * game) {

	if (game != NULL) {

		Player_destroy(game->player);

		Node * actual = game->enemies->begin;
		Node * aux = NULL;

		while (actual != NULL) {

			aux = actual->next;

			if (actual->value != NULL) {
				Enemy_destroy((Enemy *) actual->value);
			}
			actual = aux;
		}

		actual = game->bullets->begin;

		while (actual != NULL) {

			aux = actual->next;

			if (actual->value != NULL) {
				Bullet_destroy((Bullet *) actual->value);
			}
			actual = aux;
		}

		List_destroy(game->enemies);
		List_destroy(game->bullets);
		free(game);
	}

	Timer_destroy(game->timer);
}

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
		game->oxygen_bar = OxygenBar_create(window);
		game->player = Player_create(window, RES_SUBMARINE, 2 * MOVEMENT_FACTOR,
		TIME_BETWEEN_SHOTS);
		game->enemies = List_create();
		game->bullets = List_create();
		game->divers = List_create();
		game->window = window;
		game->enemies_on_screen = 0;
		game->divers_on_screen = 0;
		game->is_paused = false;
		game->surface = SDL_GetWindowSurface(window);

		SDL_Rect breathe_zone = { 0, 0, game->surface->w,
				(game->surface->h >> 3) };

		game->breathe_zone = breathe_zone;
		game->timer = Timer_create();

		game->explosion_sound = Mix_LoadWAV(RES_EXPLOSION_SOUND);

		Timer_start(game->timer);
	}
	return game;
}

bool Game_is_player_breathing(Game * game) {

	SDL_Rect rect;
	SDL_bool b = SDL_IntersectRect(&game->breathe_zone, game->player->rect,
			&rect);

	return b ? true : false;
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

Diver * Game_spawn_diver(Game * game, Direction direction, int y,
		float movement_factor) {

	Diver * diver = NULL;

	if (game->divers_on_screen < MAX_DIVERS_ON_SCREEN) {
		diver = Diver_create(game->window, movement_factor, direction, y);
		List_insert(game->divers, diver);
		game->divers_on_screen++;
	}

	return diver;
}

void Game_update(Game * game) {

	SDL_FillRect(game->surface, NULL,
			SDL_MapRGB(game->surface->format, 0x00, 0x66, 0xFF));

	Player_render(game->player, game->surface);

	if (!game->is_paused) {
		if (!Game_is_player_breathing(game)) {
			if (game->player->oxygen >= 0) {
				game->player->oxygen -= 0.04;

				if (game->player->oxygen <= 0) {
					game->player->oxygen = 0.0;
				}
				OxygenBar_set_oxygen(game->oxygen_bar, game->player->oxygen);
			}
		} else {
			if (game->player->oxygen < 100) {
				game->player->oxygen += 0.15;
				if (game->player->oxygen > 100)
					game->player->oxygen = 100;
				OxygenBar_set_oxygen(game->oxygen_bar, game->player->oxygen);
			}
		}
	}

	OxygenBar_render(game->oxygen_bar, game->surface);
	Game_update_divers(game);
	Game_update_enemies(game);
	Game_update_bullets(game);

	Game_check_bullets_collision(game);
	Game_check_divers_collision(game);

	SDL_UpdateWindowSurface(game->window);
}

void Game_check_divers_collision(Game * game) {
	if (!game->is_paused) {

		Node * node = game->divers->begin;
		Node * aux = NULL;

		while (node != NULL) {
			aux = node->next;

			Diver * diver = (Diver *) node->value;

			bool collision = collision_check(diver->rect, diver->collision_mask,
					game->player->rect, game->player->collision_mask);

			if (collision) {
				Game_destroy_diver(game, diver);
				game->player->divers_rescued++;
			}
			node = aux;
		}
	}
}


void Game_update_enemies(Game * game) {
Node * actual = game->enemies->begin;

while (actual != NULL) {
	if (actual->value != NULL) {
		Node * prox = actual->next;
		Enemy * enemy = (Enemy *) actual->value;

		actual = prox;

		if (!game->is_paused) {
			Enemy_move(enemy);
		}

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

	if (!game->is_paused) {
		Bullet_move(bullet);
	}

	if (Bullet_is_visible(bullet)) {
		Bullet_render(bullet, game->surface);
	} else {
		List_remove(game->bullets, bullet);
		Bullet_destroy(bullet);
	}
}
}

void Game_check_bullets_collision(Game * game) {

if (!game->is_paused) {

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

				if (enemy->type == SUBMARINE) {
					Mix_PlayChannel(-1, game->explosion_sound, 0);
				}

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
}

void Game_update_divers(Game * game) {
Node * actual = game->divers->begin;

while (actual != NULL) {

	if (actual->value != NULL) {
		Node * prox = actual->next;
		Diver * diver = (Diver *) actual->value;

		actual = prox;

		if (!game->is_paused) {
			Diver_move(diver);
		}

		if (Diver_is_visible(diver)) {
			Diver_render(diver, game->surface);
		} else {
			Game_destroy_diver(game, diver);
		}

	} else {
		actual = actual->next;
	}
}
}

void Game_destroy_diver(Game * game, Diver * diver) {
List_remove(game->divers, (void *) diver);
Diver_destroy(diver);

game->divers_on_screen--;
}

void Game_destroy_divers(Game * game) {
Node * node = game->divers->begin;
Node * aux = NULL;

while (node != NULL) {
	aux = node->next;

	if (node->value != NULL) {
		Diver_destroy((Diver *) node->value);
	}

	node = aux;
}

game->divers_on_screen = 0;

game->divers = NULL;
}

void Game_destroy_enemies(Game* game) {
Node* actual = game->enemies->begin;
Node * aux = NULL;
while (actual != NULL) {

	aux = actual->next;

	if (actual->value != NULL) {
		Enemy_destroy((Enemy *) actual->value);
	}
	actual = aux;
}

game->enemies_on_screen = 0;

game->enemies = NULL;
}

void Game_destroy_bullets(Game* game) {
Node* actual = game->bullets->begin;
Node * aux = NULL;
while (actual != NULL) {

	aux = actual->next;

	if (actual->value != NULL) {
		Bullet_destroy((Bullet *) actual->value);
	}
	actual = aux;
}

game->enemies = NULL;
}

void Game_destroy(Game * game) {

if (game != NULL) {

	Player_destroy(game->player);

	Game_destroy_enemies(game);
	Game_destroy_bullets(game);
	Game_destroy_divers(game);
	Node * actual = game->bullets->begin;
	Node * aux = NULL;

	while (actual != NULL) {

		aux = actual->next;

		if (actual->value != NULL) {
			Bullet_destroy((Bullet *) actual->value);
		}
		actual = aux;
	}
	List_destroy(game->enemies);
	List_destroy(game->bullets);
	List_destroy(game->divers);
	Mix_FreeChunk(game->explosion_sound);
	Timer_destroy(game->timer);
	free(game);
}

}

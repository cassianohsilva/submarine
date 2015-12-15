/*
 * game.c
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "game.h"

#define DEFAULT_VELOCITY_FACTOR 1.0

#define DIVER_RESCUE_SCORE 60
#define ENEMY_DESTROY_SCORE 60

int zone_to_screen(Game * game, int zone);
int screen_to_zone(Game * game, int y);

void on_click_start(void * data) {
	if (data) {
		Game * game = (Game *) data;
		Game_start(game);
	}
}

void on_click_resume(void * data) {
	if (data) {
		Game * game = (Game *) data;
		game->is_paused = false;
	}
}

void on_click_quit(void * data) {
	if (data) {
		Game_destroy((Game *) data);
	}

	exit(0);
}

void on_click_exit(void * data) {
	if (data) {
		Game_stop((Game *) data);
		Game_reset((Game *) data);
	}
}

Game * Game_create(SDL_Window * window) {
	Game * game = (Game *) malloc(sizeof(Game));

	if (game != NULL) {
		game->oxygen_bar = OxygenBar_create(window);
		game->player = Player_create(window, RES_SUBMARINE, (SCREEN_WIDTH - 15) / 2, (SCREEN_HEIGHT - 64) / 2, 2 * MOVEMENT_FACTOR,
		TIME_BETWEEN_SHOTS);
		game->enemies = List_create();
		game->bullets = List_create();
		game->divers = List_create();
		game->window = window;
		game->enemies_on_screen = 0;
		game->divers_on_screen = 0;
		game->is_paused = false;
		game->surface = SDL_GetWindowSurface(window);

		game->score_color = (SDL_Color ) {0xFF, 0xFF, 00};

		SDL_Rect breathe_zone = { 0, 0, game->surface->w,
				(game->surface->h >> 3) };

		game->breathe_zone = breathe_zone;
		game->timer = Timer_create();

		game->spawn_zone_size = (SCREEN_HEIGHT - game->breathe_zone.h)
				/ game->player->sprite_rect->h;

		game->zone_lock = (ZoneLock *) malloc(
				game->spawn_zone_size * sizeof(ZoneLock));

		int i;

		for (i = 0; i < game->spawn_zone_size; i++) {
			game->zone_lock[i] = (ZoneLock ) {false, 0, 0, 0};
		}

		game->explosion_sound = Mix_LoadWAV(RES_EXPLOSION_SOUND);
		game->rescue_sound = Mix_LoadWAV(RES_RESCUE_DIVER_SOUND);

		game->background_music = Mix_LoadMUS(RES_BACKGROUND_SOUND);

		Timer_start(game->timer);

		game->font = TTF_OpenFont(RES_DEFAULT_FONT, 28);

		game->score_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

		game->is_started = false;

		char temp[20];
		sprintf(temp, "%d", game->player->score);

		game->score_surface = TTF_RenderText_Solid(game->font, temp,
				game->score_color);

		if (game->score_rect) {
			game->score_rect->x = (SCREEN_WIDTH - game->score_surface->w) / 2;
			game->score_rect->y = 0;
			game->score_rect->w = game->score_surface->w;
			game->score_rect->h = game->score_surface->h;
		}

		SDL_Color color = { 0x33, 0x33, 0x33, 0xFF / 2 };

		game->pause_menu = Menu_create(window, NULL, color);
		game->main_menu = Menu_create(window, NULL, color);

		if (game->pause_menu) {

			Button * resume_button = Button_create(window, RES_RESUME,
					on_click_resume);
			Button * quit_button = Button_create(window, RES_QUIT,
					on_click_exit);

			Button_set_postition(resume_button,
					(SCREEN_WIDTH - resume_button->rect->w) / 2,
					(int) (SCREEN_HEIGHT / 2 - resume_button->rect->w * 1.2));
			Button_set_postition(quit_button,
					(SCREEN_WIDTH - quit_button->rect->w) / 2,
					(int) (SCREEN_HEIGHT / 2 + quit_button->rect->w * 1.2));

			Menu_add_button(game->pause_menu, resume_button);
			Menu_add_button(game->pause_menu, quit_button);
		}

		if (game->main_menu) {

			Button * start_button = Button_create(window, RES_RESUME,
					on_click_start);
			Button * quit_button = Button_create(window, RES_QUIT,
					on_click_quit);

			Button_set_postition(start_button,
					(SCREEN_WIDTH - start_button->rect->w) / 2,
					(int) (SCREEN_HEIGHT / 2 - start_button->rect->w * 1.2));
			Button_set_postition(quit_button,
					(SCREEN_WIDTH - quit_button->rect->w) / 2,
					(int) (SCREEN_HEIGHT / 2 + quit_button->rect->w * 1.2));

			Menu_add_button(game->main_menu, start_button);
			Menu_add_button(game->main_menu, quit_button);
		}
	}
	return game;
}

void Game_reset(Game * game) {
	if (game) {
		game->enemies_on_screen = 0;
		game->divers_on_screen = 0;
		game->player->score = 0;

		Player_set_position(game->player, (SCREEN_WIDTH - 15) / 2, (SCREEN_HEIGHT - 64) / 2);

		int i;

		for (i = 0; i < game->spawn_zone_size; i++) {
			game->zone_lock[i] = (ZoneLock ) {false, 0, 0, 0};
		}

		Game_destroy_bullets(game);
		Game_destroy_enemies(game);
		Game_destroy_divers(game);
	}
}

void Game_update_score_surface(Game * game) {
	char temp[20];
	sprintf(temp, "%d", game->player->score);

	SDL_FreeSurface(game->score_surface);

	game->score_surface = TTF_RenderText_Solid(game->font, temp,
			game->score_color);

	game->score_rect->x = (SCREEN_WIDTH - game->score_surface->w) / 2;
	game->score_rect->w = game->score_surface->w;
	game->score_rect->h = game->score_surface->h;
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

Enemy * Game_spawn_enemy(Game * game) {

	Enemy * enemy = NULL;

	float probability = ((float) rand()) / INT32_MAX;

	if (probability < 0.01) {
		EnemyType enemy_type = (rand() > (INT32_MAX >> 1)) ? SUBMARINE : SHARK;
		Direction direction = (rand() > (INT32_MAX >> 1)) ? RIGHT : LEFT;

		Uint8 zone = rand() % game->spawn_zone_size;

		int y = zone_to_screen(game, zone);

		if (game->enemies_on_screen < MAX_ENEMIES_ON_SCREEN
				&& !game->zone_lock[zone].is_locked
				&& (game->zone_lock[zone].direction == 0
						|| game->zone_lock[zone].direction == direction)) {
			if (enemy_type == SHARK) {
				enemy = Enemy_create(game->window, RES_SHARK, enemy_type,
						direction, y, DEFAULT_VELOCITY_FACTOR, 0);
			} else if (enemy_type == SUBMARINE) {
				enemy = Enemy_create(game->window, RES_ENEMY_SUBMARINE,
						enemy_type, direction, y, DEFAULT_VELOCITY_FACTOR,
						TIME_BETWEEN_SHOTS);
			}

			if (enemy != NULL) {
				List_insert(game->enemies, (void *) enemy);
				game->enemies_on_screen++;
			}

			if (game->zone_lock[zone].enemies_number == 0) {
				game->zone_lock[zone].is_locked = true;
				game->zone_lock[zone].direction = direction;
				game->zone_lock[zone].enemy_type = enemy_type;
			}

			game->zone_lock[zone].enemies_number++;
		}
	}

	return enemy;
}

void Game_destroy_enemy(Game * game, Enemy * enemy) {

	int zone = screen_to_zone(game, enemy->rect->y);

	game->zone_lock[zone].enemies_number--;

	if (enemy->rect->x + enemy->rect->w >= SCREEN_WIDTH) {
		game->zone_lock[zone].is_locked = false;
	}

	List_remove(game->enemies, (void *) enemy);
	Enemy_destroy(enemy);

	game->enemies_on_screen--;
}

void Game_destroy_bullet(Game * game, Bullet * bullet) {
	List_remove(game->bullets, (void *) bullet);
	Bullet_destroy(bullet);
}

Diver * Game_spawn_diver(Game * game) {

	Diver * diver = NULL;
	float probability = ((float) rand()) / INT32_MAX;

	if (probability < 0.005) {
		Direction direction = (rand() > (INT32_MAX >> 1)) ? RIGHT : LEFT;

		Uint8 zone = rand() % game->spawn_zone_size;

		int y = zone_to_screen(game, zone) + game->player->surface->h / 4;

		if (game->divers_on_screen < MAX_DIVERS_ON_SCREEN) {
			diver = Diver_create(game->window, DEFAULT_VELOCITY_FACTOR,
					direction, y);
			List_insert(game->divers, diver);
			game->divers_on_screen++;
		}
	}

	return diver;
}

void Game_start(Game * game) {
	if (game) {
		game->is_started = true;
	}
}

void Game_stop(Game * game) {
	if (game) {
		game->is_started = false;
		game->is_paused = false;
	}
}

void Game_update(Game * game) {

	SDL_FillRect(game->surface, NULL,
			SDL_MapRGB(game->surface->format, 0x00, 0x66, 0xFF));

	if (game->is_started) {

		Player_render(game->player, game->surface);

		if (!game->is_paused) {
			if (!Game_is_player_breathing(game)) {
				if (game->player->oxygen >= 0) {
					game->player->oxygen -= 0.04;

					if (game->player->oxygen <= 0) {
						game->player->oxygen = 0.0;
					}
				}

			} else {
				if (game->player->oxygen < 100) {
					game->player->oxygen += 0.15;
					if (game->player->oxygen > 100)
						game->player->oxygen = 100;
				}

				if (game->player->divers_rescued == MAX_DIVERS_FOR_RESCUE) {
					game->player->divers_rescued = 0;
					game->player->score += DIVER_RESCUE_SCORE;

					Game_update_score_surface(game);
				}
			}
			OxygenBar_set_oxygen(game->oxygen_bar, game->player->oxygen);

			if (!Mix_PlayingMusic()) {
				Mix_PlayMusic(game->background_music, -1);
			} else if (Mix_PausedMusic()) {
				Mix_ResumeMusic();
			}
		} else {
			if (Mix_PlayingMusic()) {
				Mix_PauseMusic();
			}
		}

		Game_update_divers(game);
		Game_update_enemies(game);

		Game_update_bullets(game);
		OxygenBar_render(game->oxygen_bar, game->surface);


		SDL_BlitSurface(game->score_surface, NULL, game->surface,
				game->score_rect);

		if (game->is_paused) {
			Menu_render(game->pause_menu, game->surface);
		}

		Game_check_bullets_collision(game);
		Game_check_divers_collision(game);
	} else {
		Menu_render(game->main_menu, game->surface);
	}

	SDL_UpdateWindowSurface(game->window);
}

void Game_check_divers_collision(Game * game) {
	if (!game->is_paused) {

		Node * node = game->divers->begin;
		Node * aux = NULL;

		if (game->player->divers_rescued < MAX_DIVERS_FOR_RESCUE) {
			while (node != NULL) {
				aux = node->next;

				Diver * diver = (Diver *) node->value;

				bool collision = collision_check(diver->rect,
						diver->collision_mask, game->player->rect,
						game->player->collision_mask);

				if (collision) {
					Game_destroy_diver(game, diver);
					Mix_PlayChannel(-1, game->rescue_sound, 0);
					game->player->divers_rescued++;
				}
				node = aux;
			}
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

			if (enemy->rect->x + enemy->rect->w < SCREEN_WIDTH) {
				game->zone_lock[screen_to_zone(game, enemy->rect->y)].is_locked =
						false;
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
						bullet->collision_mask, enemy->rect,
						enemy->collision_mask);

				if (collision) {
					if (enemy->type == SUBMARINE) {
						Mix_PlayChannel(-1, game->explosion_sound, 0);
					}

					Game_destroy_enemy(game, enemy);
					Game_destroy_bullet(game, bullet);

					game->player->score += ENEMY_DESTROY_SCORE;

					Game_update_score_surface(game);

					break;
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

	game->divers->begin = NULL;

	game->divers_on_screen = 0;
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

	game->enemies->begin = NULL;

	game->enemies_on_screen = 0;
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

	game->bullets->begin = NULL;
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

		SDL_FreeSurface(game->score_surface);
		free(game->score_rect);
		free(game->zone_lock);

		TTF_CloseFont(game->font);
		Mix_FreeChunk(game->explosion_sound);
		Mix_FreeChunk(game->rescue_sound);
		Mix_FreeMusic(game->background_music);
		Timer_destroy(game->timer);
		Menu_destroy(game->pause_menu);
		Menu_destroy(game->main_menu);
		free(game);
	}
}

int zone_to_screen(Game * game, int zone) {
	return game->player->surface->h * zone + game->breathe_zone.h;
}

int screen_to_zone(Game * game, int y) {
	return (y - game->breathe_zone.h) / game->player->surface->h;
}

/*
 * player.h
 *
 *  Created on: 30 de out de 2015
 *      Author: cassiano
 */

#include "types.h"
#include "bullet.h"
#include "linked_list.h"

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

typedef struct _Player {
	SDL_Window * window;
	SDL_Rect * rect;
	int oxygen;
	SDL_Rect * sprite_rect;
	SDL_Surface * surface;
	Direction look_dir;

	float movement_factor;
	List * bullet_list;
} Player;

/**
 * Cria um jogador carregando uma imagem na posição (0, 0)
 */
Player * Player_create(SDL_Window * window, const char * filename, float movement_factor);

/**
 * Renderiza o player na SDL_Surface pai
 */
void Player_render(const Player* player, SDL_Surface* parent);

/**
 * Move o player na horizontal e na vertical
 */
void Player_move(Player *, int, int, int, int);

void Player_destroy(Player *);

#endif /* SRC_PLAYER_H_ */

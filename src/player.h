/*
 * player.h
 *
 *  Created on: 30 de out de 2015
 *      Author: cassiano
 */

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

typedef struct _Player {
	SDL_Surface *surface;
	SDL_Rect *rect;
} Player;

/**
 * Cria um jogador carregando uma imagem na posição (0, 0)
 */
Player * Player_create(const char *);

/**
 * Renderiza o player na SDL_Surface pai
 */
void Player_render(const Player *player, SDL_Surface * parent);

/**
 * Move o player na horizontal e na vertical
 */
void Player_move(const Player *, int, int, int, int);

void Player_destroy(Player *);

#endif /* SRC_PLAYER_H_ */

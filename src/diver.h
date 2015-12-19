/*
 * diver.h
 *
 *  Created on: 26 de nov de 2015
 *      Author: cassiano
 */

#include "types.h"

#ifndef SRC_DIVER_H_
#define SRC_DIVER_H_

typedef struct {
	SDL_Window * window;
	SDL_Surface * surface;
	SDL_Rect * rect;
	SDL_Rect * sprite_rect;
	float real_x;
	float movement_factor;
	Direction direction;

	CollisionMask collision_mask;
} Diver;

Diver * Diver_create(SDL_Window * window, float movement_factor, Direction direction, int y_position);

void Diver_move(Diver * diver);

bool Diver_is_visible(Diver * diver);

void Diver_change_direction(Diver * diver);

void Diver_render(Diver * diver, SDL_Surface * parent);

void Diver_destroy(Diver * diver);

#endif /* SRC_DIVER_H_ */

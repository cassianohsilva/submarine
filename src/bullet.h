/*
 * shot.h
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "types.h"

#define MAX_BULLETS 10

#ifndef SRC_SHOT_H_
#define SRC_SHOT_H_

typedef struct {
	Direction direction;
	float movement_factor;
	CollisionMask collision_mask;

	SDL_Surface * surface;
	SDL_Rect * rect;
	SDL_Window * window;
} Bullet;

Bullet * Bullet_create(SDL_Window * window, Direction direction, float velocity_factor, int x, int y, CollisionMask collision_mask,
		const char * filename);

void Bullet_render(Bullet * bullet, SDL_Surface * parent);

void Bullet_move(Bullet * bullet);

bool Bullet_is_visible(Bullet * bullet);

void Bullet_destroy(Bullet * bullet);

#endif /* SRC_SHOT_H_ */

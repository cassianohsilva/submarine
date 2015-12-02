/*
 * types.h
 *
 *  Created on: 12 de nov de 2015
 *      Author: cassiano
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "resources.h"

#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

typedef enum {
	LEFT = -1, RIGHT = 1
} Direction;

typedef enum {
	PLAYER_LAYER = 1, ENEMY_LAYER = 2, DIVER_LAYER = 4
} CollisionMask;

typedef enum {
	SUBMARINE = 1, SHARK = 0
} EnemyType;

typedef enum {
	true = 1, false = 0
} bool;

#endif /* SRC_TYPES_H_ */

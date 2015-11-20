/*
 * types.h
 *
 *  Created on: 12 de nov de 2015
 *      Author: cassiano
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "resources.h"

#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

typedef enum {
	LEFT = 5, RIGHT = 12
} Direction;

typedef enum {
	SUBMARINE = 10, SHARK = 17
} EnemyType;

typedef enum {
	true = 1, false = 0
} bool;

#endif /* SRC_TYPES_H_ */

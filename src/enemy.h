#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef SRC_ENEMY_H_
#define SRC_ENEMY_H_

#include "types.h"

typedef struct {
	SDL_Window * window;
	SDL_Rect * rect;
	SDL_Rect * sprite_rect;
	SDL_Surface * surface;
	Direction look_dir;
} Enemy;

Enemy * Enemy_create(SDL_Window * window, const char * filename);

void Enemy_render(const Enemy * enemy, SDL_Surface * parent);

void Enemy_move(Enemy * enemy, int h_move, int v_move, int x_max, int y_max);

bool Enemy_is_visible(Enemy * enemy);

void Enemy_destroy(Enemy * enemy);

#endif /* SRC_ENEMY_H_ */


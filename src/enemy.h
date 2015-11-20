#include "types.h"

#ifndef SRC_ENEMY_H_
#define SRC_ENEMY_H_

typedef struct {

} Shark;

typedef struct {

} EnemySubmarine;

typedef struct {
	SDL_Window * window;
	SDL_Rect * rect;
	SDL_Rect * sprite_rect;
	SDL_Surface * surface;
	Direction look_dir;
	float velocity_factor;

	EnemyType type;
	void * real_enemy;

} Enemy;

Enemy * Enemy_create(SDL_Window * window, const char * filename, EnemyType type);

void Enemy_render(const Enemy * enemy, SDL_Surface * parent);

void Enemy_move(Enemy * enemy, int h_move, int v_move);

bool Enemy_is_visible(Enemy * enemy);

void Enemy_destroy(Enemy * enemy);

#endif /* SRC_ENEMY_H_ */


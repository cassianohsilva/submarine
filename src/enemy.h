#include "types.h"
#include "bullet.h"
#include "linked_list.h"

#ifndef SRC_ENEMY_H_
#define SRC_ENEMY_H_

typedef struct {

} Shark;

typedef struct {
	List * bullet_list;
	Uint32 time_between_shots;
	Uint32 time_shot_counter;
} EnemySubmarine;

typedef struct {
	SDL_Window * window;
	SDL_Rect * rect;
	SDL_Rect * sprite_rect;
	SDL_Surface * surface;
	Direction direction;
	float movement_factor;

	EnemyType type;
	void * real_enemy;

} Enemy;

Enemy * Enemy_create(SDL_Window * window, const char * filename, EnemyType type,
		Direction direction, int y, float velocity_factor,
		int time_between_shots);

void Enemy_render(const Enemy * enemy, SDL_Surface * parent);

void Enemy_move(Enemy * enemy);

bool Enemy_is_visible(Enemy * enemy);

void Enemy_destroy(Enemy * enemy);

#endif /* SRC_ENEMY_H_ */


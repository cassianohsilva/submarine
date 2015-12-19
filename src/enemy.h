#include "types.h"
#include "bullet.h"
#include "linked_list.h"

#ifndef SRC_ENEMY_H_
#define SRC_ENEMY_H_

typedef struct {

} Shark;

typedef struct {
	Uint32 time_between_shots;
	Uint32 time_shot_counter;
} EnemySubmarine;

typedef struct {
	SDL_Window * window;
	SDL_Rect * rect;
	SDL_Rect * sprite_rect;
	int default_x;
	float aux_x;
	SDL_Surface * surface;
	bool entered_on_screen;
	Direction direction;
	float movement_factor;

	CollisionMask collision_mask;
	EnemyType type;
	void * real_enemy;

} Enemy;

Enemy * Enemy_create(SDL_Window * window, const char * filename, EnemyType type,
		Direction direction, int y, float velocity_factor,
		int time_between_shots);

void Enemy_render(const Enemy * enemy, SDL_Surface * parent, List * bullets);

void Enemy_move(Enemy * enemy);

bool Enemy_is_visible(Enemy * enemy);

bool Enemy_is_entered_on_screen(Enemy * enemy);

void Enemy_destroy(Enemy * enemy);

#endif /* SRC_ENEMY_H_ */


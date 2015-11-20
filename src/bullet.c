/*
 * shot.c
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "bullet.h"

Bullet * Bullet_create(SDL_Window * window, Direction direction, float velocity_factor, int x, int y,
		const char * filename) {

	Bullet * bullet = (Bullet *) malloc(sizeof(Bullet));

	if(bullet != NULL) {
		bullet->window = window;
		bullet->surface = IMG_Load(filename);

		if(bullet->surface != NULL) {

			bullet->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			bullet->rect->x = x;
			bullet->rect->y = y;
			bullet->rect->w = bullet->surface->w;
			bullet->rect->h = bullet->surface->h;

			bullet->direction = direction;
			bullet->velocity_factor = velocity_factor;
		} else {
			printf("Erro ao carregar a imagem \'%s\': %s\n", filename, IMG_GetError());
		}
	}

	return bullet;
}

void Bullet_render(Bullet * bullet, SDL_Surface * parent) {
	SDL_BlitSurface(bullet->surface, NULL, parent, bullet->rect);
}

void Bullet_move(Bullet * bullet) {
	if(bullet != NULL) {
		bullet->rect->x += (int) bullet->direction * bullet->velocity_factor;
	}
}

bool Bullet_is_visible(Bullet * bullet) {
	//TODO Implementar essa função
	return false;
}

void Bullet_destroy(Bullet * bullet) {
	free(bullet->surface);
	free(bullet->rect);
	free(bullet);
}

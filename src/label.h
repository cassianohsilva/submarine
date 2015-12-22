/*
 * label.h
 *
 *  Created on: 22 de dez de 2015
 *      Author: cassiano
 */
#include "types.h"

#ifndef SRC_LABEL_H_
#define SRC_LABEL_H_

typedef struct {
	SDL_Window * window;
	SDL_Surface * surface;
	SDL_Rect rect;
	char * text;
} Label;

Label * Label_create(SDL_Window * window, TTF_Font * font, char * text, int x, int y);

void Label_render(Label * label, SDL_Surface * parent);

void Label_destroy(Label * label);

#endif /* SRC_LABEL_H_ */

/*
 * input.h
 *
 *  Created on: 15 de dez de 2015
 *      Author: cassiano
 */

#include "types.h"

#ifndef SRC_INPUT_H_
#define SRC_INPUT_H_

typedef struct {
	SDL_Window * window;
	SDL_Surface * surface;
	SDL_Surface * text_surface;
	SDL_Rect * rect;
	TTF_Font * font;
	SDL_Color background_color;
	SDL_Color input_color;
	char * text;
} Input;

Input * Input_create(SDL_Window * window, TTF_Font * font, SDL_Rect * rect,
		SDL_Color background_color, SDL_Color input_color);

void Input_set_text(Input * input, char * text);

void Input_insert_text(Input * input, char * text);

void Input_render(Input * input, SDL_Surface * parent);

void Input_destroy(Input * input);

#endif /* SRC_INPUT_H_ */

#include "types.h"

#ifndef SRC_BUTTON_H_
#define SRC_BUTTON_H_

typedef struct {
	SDL_Window * window;
	SDL_Rect * rect;
	SDL_Surface * surface;
	void (* on_click)(void * data);
} Button;

Button * Button_create(SDL_Window * window, const char * filename, void (* on_click)(void * data));

void Button_render(Button * button, SDL_Surface* parent);

bool Button_was_click(Button * button, int x, int y);

void Button_on_click(Button * button, void * data);

void Button_set_postition(Button * button, int x, int y);

void Button_destroy(Button * button);

#endif

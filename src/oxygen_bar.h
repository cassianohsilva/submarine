#include "types.h"

typedef struct{
	SDL_Window * window;
	SDL_Rect rect;
	SDL_Rect oxygen_rect;
	SDL_Surface * surface;

}OxygenBar;

OxygenBar * OxygenBar_create(SDL_Window * window);

void OxygenBar_destroy(OxygenBar * oxygenbar);

void OxygenBar_render(OxygenBar* oxygenbar, SDL_Surface* parent);

void OxygenBar_set_oxygen(OxygenBar * oxygenbar, float oxygen);





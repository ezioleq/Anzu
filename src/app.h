#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>

#include "config.h"

typedef struct App {
	int argc;
	char **argv;
	Config cfg;
	bool running;

	Display *x_display;
	Window x_window;
	XWindowAttributes x_attribs;
	XImage *image;

	SDL_Window *window;
	SDL_Event evt;
	SDL_Surface *window_surface;
	SDL_Surface *screen_surface;

	int off_x;
	int off_y;
	int scr_w;
	int scr_h;
} App;

void app_run(App *app, int argc, char** argv);
void app_init(App *app);
void app_update(App *app);
void app_draw(App *app);
void app_close(App *app);

#endif

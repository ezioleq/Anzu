#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>

#include "config.h"
#include "clipboard.h"

typedef struct App {
	int argc;
	char **argv;
	Config cfg;
	Clipboard clipboard;
	bool running;

	Display *x_display;
	Window x_window;
	XWindowAttributes x_attribs;
	XImage *image;
	XEvent x_evt;

	SDL_Window *window;
	SDL_Event evt;
	SDL_Surface *window_surface;
	SDL_Surface *screen_surface;

	int screen_w;
	int screen_h;
} App;

void app_run(App *app, int argc, char** argv);
void app_init(App *app);
void app_handle_events(App *app);
void app_update(App *app);
void app_draw(App *app);
void app_close(App *app);

#endif

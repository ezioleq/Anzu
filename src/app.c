#include "app.h"

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "save.h"

void app_run(App *app, int argc, char** argv) {
	app->running = true;
	app->argc = argc;
	app->argv = argv;

	app_init(app);
	while (app->running) {
		app_handle_events(app);
		app_update(app);
		app_draw(app);
	}
	app_close(app);
}

void app_init(App *app) {
	const char *help_string =
		"usage: anzu [options]\n"
		"Fast and simple screenshooter for X11\n"
		"options:\n"
		"  -d <time>      Set screenshot delay in milliseconds\n"
		"  -s <path>      Save image to specified directory\n"
		"  -f <format>    Set proper C date format for filename (default is %Y-%m-%d-%H-%M-%S)\n"
		"  -n             Capture entire screen without starting GUI\n"
		"  -h             Show help\n";

	config_init(&app->cfg);

	int opt;
	extern char *optarg;

	while ((opt = getopt(app->argc, app->argv, ":hnd:s:f:")) != -1) {
		switch (opt) {
			case 'h': /* Help */
				printf("%s", help_string);
				exit(EXIT_SUCCESS);
				break;

			case 'd': /* Delay */
				app->cfg.delay = atoi(optarg);
				break;

			case 's': /* Save path */
				app->cfg.save_path = malloc(strlen(optarg)+1);
				strcpy(app->cfg.save_path, optarg);
				app->cfg.is_save_path_set = true;
				break;

			case 'f': /* Date format */
				app->cfg.date_format = malloc(strlen(optarg)+1);
				strcpy(app->cfg.date_format, optarg);
				app->cfg.is_date_format_set = true;
				break;

			case 'n': /* No GUI */
				app->cfg.nogui = true;
				break;
		}
	}

	/* Set default filename date format */
	if (!app->cfg.is_date_format_set) {
		const char *default_format = "%Y-%m-%d-%H-%M-%S";
		app->cfg.date_format = malloc(strlen(default_format)+1);
		strcpy(app->cfg.date_format, default_format);
	}

	/* TODO: Add params for setting this */
	app->off_x = app->off_y = 0;

	/* Our screenshot delay */
	SDL_Delay(app->cfg.delay);

	/* Screen capturing */
	app->x_display = XOpenDisplay(NULL);
	app->x_window = DefaultRootWindow(app->x_display);
	XGetWindowAttributes(app->x_display, app->x_window, &app->x_attribs);

	app->scr_w = app->x_attribs.width;
	app->scr_h = app->x_attribs.height;

	app->image = XGetImage(
		app->x_display, app->x_window,
		app->off_x, app->off_y,
		app->scr_w, app->scr_h,
		AllPlanes, ZPixmap
	);

	SDL_Surface *temp = SDL_CreateRGBSurfaceWithFormatFrom(
		app->image->data, app->scr_w, app->scr_h, 32,
		app->image->width*4, SDL_PIXELFORMAT_RGB888
	);

	app->screen_surface = SDL_CreateRGBSurfaceWithFormat(
		0, temp->w, temp->h,
		temp->format->BitsPerPixel,
		temp->format->format
	);

	SDL_BlitSurface(temp, 0, app->screen_surface, 0);
	SDL_FreeSurface(temp);

	clipboard_init(&app->clipboard, app->x_display, app->x_window);

	/* Logic for running without GUI */
	if (app->cfg.nogui) {
		if (app->cfg.is_save_path_set) {
			char *filename = get_filename(app->cfg.date_format);
			char *full_path = NULL;

			if (directory_exists(app->cfg.save_path))
				full_path = get_full_path(app->cfg.save_path, filename);
			else {
				fprintf(stderr, "Given directory doesn't exist, saving to the default one!\n");
				char *default_path = get_default_save_path();
				full_path = get_full_path(default_path, filename);
				free(default_path);
			}
			
			IMG_SavePNG(app->screen_surface, full_path);
			clipboard_set_data_from_file(&app->clipboard, full_path);

			fprintf(stderr, "Saved screenshot to %s\n", full_path);
		
			free(filename);
			free(full_path);
		}

		// app->running = false;
	} else {
		/* TODO: GUI */
		fprintf(stderr, "Error! There's no GUI yet!\n");
		app->running = false;
	}
}

void app_handle_events(App *app) {
	XEvent *ev = &app->x_ev;
	XNextEvent(app->x_display, ev);
	switch (ev->type) {
		case SelectionClear:
			/* We lost ownership */
			printf("Ownership lost\n");
			app->running = false;
			break;
		case SelectionRequest: {
			XSelectionRequestEvent *sev = &ev->xselectionrequest;
			clipboard_manage(&app->clipboard, app->x_display, sev);
		} break;
		default: printf("%d\n", ev->type); break;
	}
}

void app_update(App *app) {

}

void app_draw(App *app) {

}

void app_close(App *app) {
	config_free(&app->cfg);
	clipboard_close(&app->clipboard);
	XDestroyWindow(app->x_display, app->x_window);
	XCloseDisplay(app->x_display);
	XDestroyImage(app->image);
	SDL_FreeSurface(app->screen_surface);
}

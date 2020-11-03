#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>

void app_run(App *app, int argc, char** argv) {
	app->running = true;
	app->argc = argc;
	app->argv = argv;

	app_init(app);
	while (app->running) {
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

	/* Screen capturing will be here */

	SDL_Delay(app->cfg.delay);
}

void app_update(App *app) {
	app->running = false; /* We will come back here */
}

void app_draw(App *app) {

}

void app_close(App *app) {
	config_free(&app->cfg);
}

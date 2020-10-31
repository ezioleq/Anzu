#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "config.h"

int main(int argc, char **argv) {
	Config cfg;
	config_init(&cfg);

	const char *help_string =
		"usage: anzu [options]\n"
		"Fast and simple screenshooter for X11\n"
		"options:\n"
		"  -d <time>      Set screenshot delay in milliseconds\n"
		"  -s <path>      Save image to specified directory\n"
		"  -f <format>    Set proper C date format for filename (default is %Y-%m-%d-%H-%M-%S)\n"
		"  -n             Capture entire screen without starting GUI\n"
		"  -h             Show help\n";

	int opt;
	extern char *optarg;

	while ((opt = getopt(argc, argv, ":hnd:s:f:")) != -1) {
		switch (opt) {
			case 'h': /* Help */
				printf("%s", help_string);
				exit(EXIT_SUCCESS);
				break;

			case 'd': /* Delay */
				cfg.delay = atoi(optarg);
				break;

			case 's': /* Save path */
				cfg.save_path = malloc(strlen(optarg)+1);
				strcpy(cfg.save_path, optarg);
				break;

			case 'f': /* Date format */
				cfg.date_format = malloc(strlen(optarg)+1);
				strcpy(cfg.date_format, optarg);
				cfg.is_date_format_set = true;
				break;

			case 'n': /* No GUI */
				cfg.nogui = true;
				break;
		}
	}

	/* Set default filename date format */
	if (!cfg.is_date_format_set) {
		const char *default_format = "%Y-%m-%d-%H-%M-%S";
		cfg.date_format = malloc(strlen(default_format)+1);
		strcpy(cfg.date_format, default_format);
	}

	SDL_Delay(cfg.delay);

	config_free(&cfg);
}

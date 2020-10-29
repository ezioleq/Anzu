#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>

int main(int argc, char **argv) {
	int delay = 0;
	char *save_path = NULL;
	char *date_format = NULL;
	bool is_date_format_set = false; // I don't know any better way to do this
	bool nogui = false;

	int opt;
	extern char *optarg;

	while ((opt = getopt(argc, argv, ":hnd:s:f:")) != -1) {
		switch (opt) {
			case 'd': // Delay
				delay = atoi(optarg);
				break;

			case 's': // Save path
				save_path = malloc(strlen(optarg)+1);
				strcpy(save_path, optarg);
				break;

			case 'f': // Date format
				date_format = malloc(strlen(optarg)+1);
				strcpy(date_format, optarg);
				is_date_format_set = true;
				break;

			case 'n': // No GUI
				nogui = true;
				break;

			case 'h': // Help
				printf(
					"usage: Anzu [options]\n"
					"Fast and simple screenshooter\n"
					"options:\n"
					"  -d <time>      Set screenshot delay in milliseconds\n"
					"  -s <path>      Save image to specified directory\n"
					"  -f <format>    Set proper C date format for filename (default is %%Y-%%m-%%d-%%H-%%M-%%S)\n"
					"  -n             Capture entire screen without starting GUI\n"
					"  -h             Show help\n"
				);
				break;
		}
	}

	// Set default filename date format
	if (!is_date_format_set) {
		date_format = malloc(strlen("%Y-%m-%d-%H-%M-%S")+1);
		strcpy(date_format, "%Y-%m-%d-%H-%M-%S");
	}

	SDL_Delay(delay);

	free(save_path);
	free(date_format);
}

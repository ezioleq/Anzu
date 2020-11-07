#include "save.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

char *get_filename(const char *date_format) {
	time_t current_time = time(0);
	struct tm *time_info;
	
	const char buffer_size = 64;
	char *filename = malloc(buffer_size);

	time_info = localtime(&current_time);
	strftime(filename, buffer_size, date_format, time_info);
	strcat(filename, ".png");

	return filename;
}

char *get_full_path(const char *save_path, const char *filename) {
	/* It works so I won't touch this, you can change this if you want */
	const size_t sp_len = strlen(save_path);
	const size_t fn_len = strlen(filename);
	char *full_path = malloc(sp_len + fn_len + 2);
	
	memcpy(full_path, save_path, sp_len);
	memcpy(full_path + sp_len, "/", 1);
	memcpy(full_path + sp_len + 1, filename, fn_len + 1);

	return full_path;
}

char *get_default_save_path() {
	const size_t buffer_size = 1024;
	char *save_path = malloc(buffer_size);

	if (getcwd(save_path, buffer_size) == NULL) {
		save_path = getenv("HOME");
		return (save_path == NULL) ? "/tmp" : save_path;
	} else
		return save_path;
}

bool directory_exists(const char *path) {
	DIR *dir = opendir(path);
	if (dir) {
		closedir(dir);
		return true;
	} else
		return false;
}

SDL_Surface *get_cropped_surface(SDL_Surface *src, int x, int y, int w, int h) {
	
}

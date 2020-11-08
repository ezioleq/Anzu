#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

/**
 * Stuff required only by clipboard
 */
typedef struct Clipboard {
	Window owner;
	Atom targets;
	Atom sel;
	Atom save_targets;
	Atom manager;
	Atom png;
	unsigned char *data;
	size_t size;

	Display *dis;
	Window root;
} Clipboard;

/**
 * Inits a Clipboard fields to later use. 
 */
void clipboard_init(Clipboard *clip, Display *dis, Window root);

/**
 * Sends request to clipboard manager to handle data. Use it if
 * you don't have any plans with clipboard.
 */
void clipboard_close(Clipboard *clip);

/**
 * This should be used in event loop to handle X11 events.
 */
void clipboard_manage(Clipboard *clip, Display *dis, XSelectionRequestEvent *sev);

/**
 * Sets clipboard data to be sent to the requestor.
 */
void clipboard_set_data(Clipboard *clip, unsigned char *data, size_t size);

/**
 * Dirty way to handle screenshot. It is better to change this in the future.
 */
int clipboard_set_data_from_file(Clipboard *clip, const char *path);

#endif
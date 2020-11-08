#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <assert.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

/**
 * Stuff required only by clipboard
 */
typedef struct Clipboard {
	Window owner;
	Atom targets;
	Atom sel;
	Atom png;
	unsigned char *data;
	size_t size;
} Clipboard;

/**
 * Inits a Clipboard fields to later use. 
 * @param clip Pointer to Clipboard, also see return
 * @param dis Pointer to Display, make sure it is correct
 * @return 0 on success, -1 if clip is NULL 
 */
int clipboard_init(Clipboard *clip, Display *dis, Window root);

void clipboard_manage(Clipboard *clip, Display *dis, XSelectionRequestEvent *sev);

void clipboard_set_data(Clipboard *clip, unsigned char *data, size_t size);

#endif
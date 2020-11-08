#include "clipboard.h"

/**
 * We have idiots in our lives and it is the same in programming world.
 * We need to tell them about sending wrong requests and ignore it.
 */
static void send_no(Display *dis, XSelectionRequestEvent *sev) {
	XSelectionEvent ssev;

	char *atom_name = XGetAtomName(dis, sev->target);
	// printf("Invalid atom: %s\n", XGetAtomName(dis, sev->target));
	if (atom_name)
		XFree(atom_name);

	ssev.type = SelectionNotify;
	ssev.requestor = sev->requestor;
	ssev.selection = sev->selection;
	ssev.target = sev->target;
	ssev.property = None; /* <- in X11 world it means "No." */
	ssev.time = sev->time;

	XSendEvent(dis, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
}

/**
 * Sends PNG to requestor
 */
static void send_png(Display *dis, XSelectionRequestEvent *sev, Clipboard *clip) {
	char *atom_name = XGetAtomName(dis, sev->target);
	// printf("%s\n", atom_name);
	if (atom_name)
		XFree(atom_name);

	/* Change data to PNG */
	XChangeProperty(dis, sev->requestor, sev->property, clip->png, 8, PropModeReplace, clip->data, clip->size);

	XSelectionEvent ssev;
	ssev.type = SelectionNotify;
	ssev.requestor = sev->requestor;
	ssev.selection = sev->selection;
	ssev.target = sev->target;
	ssev.property = sev->property;
	ssev.time = sev->time;

	XSendEvent(dis, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
}

/** 
 * This functions answers requestor about supported targets.
 * Applications need that to know what we are supporting, e.g. image/png.
 */
static void send_targets(Display *dis, XSelectionRequestEvent *sev, Clipboard *clip) {
	char *atom_name = XGetAtomName(dis, sev->target);
	// printf("%s\n", atom_name);
	if (atom_name)
		XFree(atom_name);

	/* We are supporting "TARGETS", "SAVE_TARGETS", and "image/png" */
	Atom atoms[] = {
		clip->targets,
		clip->save_targets,
		clip->png,
	};

	XSelectionEvent ssev;
	/* Save list of supported atoms */
	XChangeProperty(dis, sev->requestor, sev->property, XA_ATOM, 32, PropModeReplace, (unsigned char*)&atoms, 3);

	ssev.type = SelectionNotify;
	ssev.requestor = sev->requestor;
	ssev.selection = sev->selection;
	ssev.target = sev->target;
	ssev.property = sev->property;
	ssev.time = sev->time;

	XSendEvent(dis, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
}

static void send_save_targets(Display *dis, XSelectionRequestEvent *sev, Clipboard *clip) {
	char *atom_name = XGetAtomName(dis, sev->target);
	// printf("%s\n", atom_name);
	if (atom_name)
		XFree(atom_name);

	/* In reality I don't know if this is right, but seems to work with clipman, so... maybe it's fine? */
	XChangeProperty(dis, sev->requestor, sev->property, XA_ATOM, 32, PropModeReplace, (unsigned char*)&clip->sel, 1);

	XSelectionEvent ssev;
	ssev.type = SelectionNotify;
	ssev.requestor = sev->requestor;
	ssev.selection = sev->selection;
	ssev.target = sev->target;
	ssev.property = sev->property;
	ssev.time = sev->time;

	XSendEvent(dis, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
}

static int wait(void *ptr) {
	SDL_Delay(2000);

	bool *done = ptr;
	*done = true;
	return 0;
}

void clipboard_init(Clipboard *clip, Display *dis, Window root) {
	/* Create empty window to receive messages */
	clip->owner = XCreateSimpleWindow(dis, root, -10, -10, 1, 1, 0, 0, 0);

	clip->dis = dis;
	clip->root = root;

	/* Get atoms */
	clip->targets = XInternAtom(dis, "TARGETS", False);
	clip->sel = XInternAtom(dis, "CLIPBOARD", False);
	clip->save_targets = XInternAtom(dis, "SAVE_TARGETS", False);
	clip->manager = XInternAtom(dis, "CLIPBOARD_MANAGER", False);
	clip->png = XInternAtom(dis, "image/png", False);

	/* Claim CLIPBOARD ownership */
	XSetSelectionOwner(dis, clip->sel, clip->owner, CurrentTime);
}

void clipboard_close(Clipboard *clip) {
	free(clip->data);
}

void clipboard_save(Clipboard *clip) {
	clip->done = false;
	Window owner = XGetSelectionOwner(clip->dis, clip->manager);

	/* Require from clipboard manager to save our data if exists, otherwise do nothing */
	if (owner != 0) {
		XConvertSelection(clip->dis, clip->manager, clip->save_targets, clip->sel, clip->root, CurrentTime);
		XFlush(clip->dis);

		/* We need to wait a bit for some clipboard managers... */
		SDL_Thread *thread = SDL_CreateThread(wait, "WaitThread", (void*)&clip->done);
		SDL_DetachThread(thread);
	}
}

void clipboard_manage(Clipboard *clip, Display *dis, XSelectionRequestEvent *sev) {
	// printf("Requestor: 0x%lx\n", sev->requestor);
	
	/* If property is None, then requestor is obsolete, so ignore. */
	if (sev->property == None) {
		send_no(dis, sev);
		return;
	}

	if (sev->target == clip->png)
		send_png(dis, sev, clip);
	else if (sev->target == clip->targets)
		send_targets(dis, sev, clip);
	else if (sev->target == clip->save_targets)
		send_save_targets(dis, sev, clip);
	else
		send_no(dis, sev);
}

void clipboard_set_data(Clipboard *clip, unsigned char *data, size_t size) {
	clip->data = data;
	clip->size = size;
}

int clipboard_set_data_from_file(Clipboard *clip, const char *path) {
	FILE *f = fopen(path, "rb");
	if (!f) return -1;

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	unsigned char *data = malloc(size);
	fread(data, sizeof(unsigned char), size, f);
	fclose(f);

	clipboard_set_data(clip, data, size);
	return 0;
}
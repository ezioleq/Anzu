#include "clipboard.h"

static void send_no(Display *dis, XSelectionRequestEvent *sev) {
	XSelectionEvent ssev;

	char *atom_name = XGetAtomName(dis, sev->target);
	printf("Invalid atom: %s\n", XGetAtomName(dis, sev->target));
	if (atom_name)
		XFree(atom_name);

	ssev.type = SelectionNotify;
	ssev.requestor = sev->requestor;
	ssev.selection = sev->selection;
	ssev.target = sev->target;
	ssev.property = None;
	ssev.time = sev->time;

	/* We need to tell requestor about wrong request */
	XSendEvent(dis, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
}

static void send_png(Display *dis, XSelectionRequestEvent *sev, Clipboard *clip) {
	char *atom_name = XGetAtomName(dis, sev->target);
	printf("%s\n", atom_name);
	if (atom_name)
		XFree(atom_name);

	XSelectionEvent ssev;
	XChangeProperty(dis, sev->requestor, sev->property, clip->png, 8, PropModeReplace, clip->data, clip->size);

	ssev.type = SelectionNotify;
	ssev.requestor = sev->requestor;
	ssev.selection = sev->selection;
	ssev.target = sev->target;
	ssev.property = sev->property;
	ssev.time = sev->time;

	/* Answer requestor */
	XSendEvent(dis, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
}

/* Some applications need also TARGETS and we need to support it */
static void send_targets(Display *dis, XSelectionRequestEvent *sev, Clipboard *clip) {
	char *atom_name = XGetAtomName(dis, sev->target);
	printf("%s\n", atom_name);
	if (atom_name)
		XFree(atom_name);

	Atom atoms[] = {
		clip->targets,
		clip->sel,
		clip->png,
	};

	XSelectionEvent ssev;
	/* Save list of atoms */
	XChangeProperty(dis, sev->requestor, sev->property, XA_ATOM, 32, PropModeReplace, &atoms, 3);

	ssev.type = SelectionNotify;
	ssev.requestor = sev->requestor;
	ssev.selection = sev->selection;
	ssev.target = sev->target;
	ssev.property = sev->property;
	ssev.time = sev->time;

	XSendEvent(dis, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
}

int clipboard_init(Clipboard *clip, Display *dis, Window root) {
	if (clip == NULL) return -1;

	/* Create window to receive messages */
	clip->owner = XCreateSimpleWindow(dis, root, -10, -10, 1, 1, 0, 0, 0);

	/* Listen for those */
	clip->targets = XInternAtom(dis, "TARGETS", False);
	clip->sel = XInternAtom(dis, "CLIPBOARD", False);
	clip->png = XInternAtom(dis, "image/png", False);

	/* Claim ownership */
	XSetSelectionOwner(dis, clip->sel, clip->owner, CurrentTime);
}

void clipboard_manage(Clipboard *clip, Display *dis, XSelectionRequestEvent *sev) {
	printf("Requestor: 0x%lx\n", sev->requestor);
	
	/* Ignore if property is None */
	if (sev->property == None) {
		send_no(dis, sev);
		return;
	}

	if (sev->target == clip->png)
		send_png(dis, sev, clip);
	/* We need to accept TARGETS request, because if not, then programs dont know we have image in clipboard */
	else if (sev->target == clip->targets)
		send_targets(dis, sev, clip);
	else if (sev->target == clip->plain)
		send_png(dis, sev, clip);
	else
		send_no(dis, sev);
}

void clipboard_set_data(Clipboard *clip, unsigned char *data, size_t size) {
	clip->data = data;
	clip->size = size;
}
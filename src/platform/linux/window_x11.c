#include "window_manager.h"
#include <stdlib.h>

/*
** Function: window_init
** ---------------------
** Initializes an X11 window and returns a populated WindowContext struct.
*/
WindowContext	*window_init(int width, int height, const char *title)
{
	WindowContext	*ctx;

	// Allocate memory for the context
	ctx = malloc(sizeof(WindowContext));
	if (!ctx) return (NULL); // Allocation failed

	// Open a connection to the X server (e.g., ":0" on local desktop)
	ctx->display = XOpenDisplay(NULL);
	if (!ctx->display)
	{
		free(ctx);
		return (NULL); // Couldn't connect to display server
	}

	// Get the default screen index (usually 0)
	ctx->screen = DefaultScreen(ctx->display);
	ctx->width = width;
	ctx->height = height;

	// Create a simple top-level window
	ctx->window = XCreateSimpleWindow(
		ctx->display,								// connection to the X server
		RootWindow(ctx->display, ctx->screen),		// parent window (desktop root)
		0, 0, width, height,						// position (x, y) and size
		1,											// border width			
		BlackPixel(ctx->display, ctx->screen),		// border color	
		WhitePixel(ctx->display, ctx->screen)		// background color
	);

	// Set the window title (for the title bar)
	XStoreName(ctx->display, ctx->window, title);

	// Choose which events to listen to (e.g. expose, key press)
	XSelectInput(ctx->display, ctx->window, ExposureMask | KeyPressMask);

	// Allow window manager to send WM_DELETE_WINDOW messages (close button pressed)
	ctx->atoms.wm_delete_window = XInternAtom(ctx->display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(ctx->display, ctx->window, &ctx->atoms.wm_delete_window, 1);

	// Make the window visible on the screen
	XMapWindow(ctx->display, ctx->window);

	// Create a graphics context (GC) for drawing
	ctx->gc = XCreateGC(ctx->display, ctx->window, 0, NULL);

	return (ctx);
}

/*
** Function: window_poll
** ---------------------
** Polls the X server for input events and processes them.
** If the user presses a key or closes the window, it sets *running to 0.
*/
void	window_poll(WindowContext *ctx, int *running)
{
	XEvent	ev;

	// Loop over all pending events in the event queue
	while (XPending(ctx->display))
	{
		XNextEvent(ctx->display, &ev); // Fetch next event
		switch (ev.type) {
			case KeyPress:
				// Any key press will signal to exit the loop
				*running = 0;
				break;
			case ClientMessage:
				// The close button has been pressed
				if ((Atom)ev.xclient.data.l[0] == ctx->atoms.wm_delete_window)
					*running = 0;
				break;
		}
	}
}

/*
** Function: window_cleanup
** ------------------------
** Frees all resources related to the window and closes the X11 connection.
*/
void	window_cleanup(WindowContext *ctx)
{
	if (!ctx) return ;

	XFreeGC(ctx->display, ctx->gc);				// Free the graphics context
	XDestroyWindow(ctx->display, ctx->window);	// Destroy the window
	XCloseDisplay(ctx->display);				// Close the connection to X server
	free(ctx);									// Free the context struct
}

#ifndef _WINDOW_MANAGER_H_
# define _WINDOW_MANAGER_H_

/*
** Platform-specific includes
** --------------------------
** Currently, we only support Linux/X11. More platforms (e.g., Windows) can be
** added later by extending this section and adapting the structs accordingly.
*/
# ifdef __linux__
#  include <X11/Xlib.h>
# endif

# ifdef _WIN32
#  include <windows.h>
# endif

/*
** Struct: X11Atoms
** ----------------
** A small container for storing common X11 Atom values used in window management.
**
** Atoms are X11's way of representing strings more efficiently. For example,
** "WM_DELETE_WINDOW" is a string, but the X server internally uses a unique ID
** (Atom) to represent it.
**
** Fields:
**     wm_delete_window - Atom identifying the "close window" protocol.
**                        This allows the window to receive a message when the user
**                        tries to close it via the window manager (e.g., clicking the X).
*/
typedef struct s_x11_atoms
{
	Atom	wm_delete_window;
}	X11Atoms;

/*
** Struct: WindowContext
** ---------------------
** This structure holds all the state and resources required to create
** and manage a window using the X11 system on Linux.
**
** Each field corresponds to a part of the X11 windowing API:
**
** int width, height:
**     - Dimensions of the window, in pixels.
**
** Platform-specific fields:
** -------------------------
** These fields are only defined for Linux/X11, but this struct is designed to
** be cross-platform. You can extend it with additional #ifdef sections
** for Windows, macOS, etc.
**
** Display *display:
**     - Represents the connection to the X server (the display server).
**     - Required for all X11 calls that manipulate windows or events.
**
** Window window:
**     - The actual X11 window object that appears on the screen.
**
** GC gc (Graphics Context):
**     - Encapsulates drawing properties (color, line width, etc.)
**     - Used in conjunction with X drawing functions like XPutImage.
**
** X11Atoms atoms:
**     - A sub-struct that stores special X11 Atoms (like the one for handling
**       window close events).
**
** int screen:
**     - Which screen to use on the display (usually screen 0).
*/
typedef struct s_window_context
{
	int			width;
	int			height;
# ifdef __linux__
	Display		*display;
	Window		window;
	GC			gc;
	X11Atoms	atoms;
	int			screen;
# endif
# ifdef _WIN32
	HWND		hwnd;
	HINSTANCE	hinstance;
	WNDCLASS	wndclass;
# endif
}	WindowContext;

/*
** Function: window_init
** ---------------------
** Creates and initializes a new window using the X11 API.
**
** Parameters:
**     width  - Width of the window in pixels.
**     height - Height of the window in pixels.
**     title  - Title of the window as shown in the window manager.
**
** Returns:
**     A pointer to a newly allocated WindowContext containing all the X11 state.
**     Returns NULL if something failed (e.g., couldn't connect to the X server).
**
** Behavior:
**     - Connects to the X server.
**     - Creates and maps a window.
**     - Sets up a graphics context.
**     - Registers for a protocol that listens for "window close" events.
*/
WindowContext	*window_init(int width, int height, const char *title);

/*
** Function: window_poll
** ---------------------
** Polls for and processes X11 events.
**
** Parameters:
**     ctx     - Pointer to the WindowContext.
**     running - A pointer to an int flag controlling the main loop.
**               If a quit event is detected, this flag will be set to 0.
**
** Behavior:
**     - Checks for pending events (like key presses, mouse clicks, window close).
**     - Processes those events one by one.
**     - Detects when the user wants to close the window (via the close button),
**       and sets the `running` flag to 0 to stop the main loop.
**
** Note:
**     This is a passive event loop — it should be called every frame from your
**     main game or application loop.
*/
void			window_poll(WindowContext *ctx, int *running);

/*
** Function: window_cleanup
** ------------------------
** Frees all resources allocated by the window system and closes the connection.
**
** Parameters:
**     ctx - Pointer to the WindowContext to be destroyed.
**
** Behavior:
**     - Frees the graphics context.
**     - Destroys the window.
**     - Closes the connection to the X server.
**     - Frees the memory used by the WindowContext itself.
**
** Required to prevent memory leaks and dangling connections to the X server.
*/
void			window_cleanup(WindowContext *ctx);

#endif /* _WINDOW_MANAGER_H_ */


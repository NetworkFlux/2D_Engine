#ifndef _RENDERER_H_
# define _RENDERER_H_

/*
** Include the X11 headers required for handling XImage structures and
** drawing to an X11 window.
*/
# include <X11/Xlib.h>
# include <X11/Xutil.h>

/*
** Include standard integer types for fixed-width data (like uint32_t).
*/
# include <stdint.h>

/*
** Include the window system header to interact with WindowContext.
*/
# include "window_manager.h"

/*
** Struct: Renderer
** ----------------
** This structure represents a simple software renderer that draws into a
** memory buffer, which is then uploaded to the screen using X11.
**
** uint32_t *buffer:
**     - Raw pixel buffer in 32-bit ARGB format.
**     - This is where all drawing operations are done in memory.
**
** XImage *image:
**     - X11 structure representing the image that will be sent to the window.
**     - Wraps around the pixel buffer for display via XPutImage.
**
** int width, height:
**     - Dimensions of the drawing buffer in pixels.
**     - Should match the window size for proper display.
*/
typedef struct {
	uint32_t	*buffer;
	XImage		*image;
	int			width;
	int			height;
}	Renderer;

/*
** Function: renderer_init
** -----------------------
** Allocates and initializes a new Renderer associated with a given window.
**
** Parameters:
**     ctx - Pointer to the WindowContext that the renderer will draw into.
**
** Returns:
**     A pointer to a new Renderer structure, or NULL on failure.
**
** Behavior:
**     - Allocates a pixel buffer in memory.
**     - Wraps the buffer in an XImage structure.
**     - Prepares everything for future drawing and presentation.
*/
Renderer	*renderer_init(WindowContext *ctx);

/*
** Function: renderer_clear
** ------------------------
** Fills the entire pixel buffer with a single color.
**
** Parameters:
**     r     - Pointer to the Renderer to operate on.
**     color - 32-bit ARGB color to fill the buffer with.
**
** Behavior:
**     - Overwrites all pixels in the buffer with the specified color.
**     - This is usually done at the beginning of each frame.
*/
void		renderer_clear(Renderer *r, uint32_t color);

/*
** Function: renderer_draw_rect
** ----------------------------
** Draws a filled rectangle in the pixel buffer.
**
** Parameters:
**     r     - Pointer to the Renderer to operate on.
**     x, y  - Top-left corner of the rectangle.
**     w, h  - Width and height of the rectangle.
**     color - 32-bit ARGB color of the rectangle.
**
** Behavior:
**     - Writes color data directly to the buffer within the specified region.
**     - Performs no bounds checking (assumes valid coordinates).
*/
void		renderer_draw_rect(Renderer *r, int x, int y, int w, int h, uint32_t color);

/*
** Function: renderer_present
** --------------------------
** Transfers the pixel buffer to the screen by calling XPutImage.
**
** Parameters:
**     r   - Pointer to the Renderer.
**     ctx - Pointer to the WindowContext where the buffer will be drawn.
**
** Behavior:
**     - Copies the pixel buffer into the visible window.
**     - Should be called once per frame after drawing.
*/
void		renderer_present(Renderer *r, WindowContext *ctx);

/*
** Function: renderer_cleanup
** --------------------------
** Frees all memory and X11 resources used by the renderer.
**
** Parameters:
**     r - Pointer to the Renderer to clean up.
**
** Behavior:
**     - Frees the pixel buffer.
**     - Destroys the XImage.
**     - Releases all renderer-related memory.
**
** Required to avoid memory leaks and dangling X11 resources.
*/
void		renderer_cleanup(Renderer *r);

#endif

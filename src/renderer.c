#include "renderer.h"

#include <stdlib.h>
#include <string.h>

/*
** Function: renderer_init
** -----------------------
** Initializes a Renderer struct tied to the given WindowContext.
*/
Renderer	*renderer_init(WindowContext *ctx)
{
	Renderer	*r;

	// Allocate memory for the renderer
	r = malloc(sizeof(Renderer));
	if (!r) return (NULL); // Allocation failed

	r->width = ctx->width;
	r->height = ctx->height;

	// Allocate a buffer for pixels (32-bit color)
	r->buffer = calloc(r->width * r->height, sizeof(uint32_t));
	if (!r->buffer)
	{
		free(r);
		return (NULL); // Buffer allocation failed
	}

	// Wrap the buffer in an XImage so it can be drawn to the window
	r->image = XCreateImage(
		ctx->display,
		DefaultVisual(ctx->display, ctx->screen),
		24, ZPixmap, 0,
		(char *)r->buffer,
		r->width, r->height,
		32, 0
	);

	return (r);
}

/*
** Function: renderer_clear
** ------------------------
** Fills the entire buffer with a given color.
*/
void	renderer_clear(Renderer *r, uint32_t color)
{
	for (int i = 0; i < r->width * r->height; i++)
		r->buffer[i] = color;
}

/*
** Function: renderer_draw_rect
** ----------------------------
** Draws a filled rectangle in the buffer.
*/
void	renderer_draw_rect(Renderer *r, int x, int y, int w, int h, uint32_t color)
{
	for (int j = 0; j < h; j++)
	{
		int py = y + j;
		if (py < 0 || py >= r->height) continue; // Clamp Y

		for (int i = 0; i < w; ++i)
		{
			int px = x + i;
			if (px < 0 || px >= r->width) continue; // Clamp X

			r->buffer[py * r->width + px] = color;
		}
	}
}

/*
** Function: renderer_present
** --------------------------
** Pushes the buffer to the screen by drawing the XImage.
*/
void	renderer_present(Renderer *r, WindowContext *ctx)
{
	XPutImage(ctx->display, ctx->window, ctx->gc, r->image, 0, 0, 0, 0, r->width, r->height);
}

/*
** Function: renderer_cleanup
** --------------------------
** Frees all resources used by the renderer.
*/
void	renderer_cleanup(Renderer *r)
{
	if (!r) return;

	XDestroyImage(r->image); // Also frees the buffer
	free(r);
}

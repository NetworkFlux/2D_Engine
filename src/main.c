#include "window_manager.h"

int	main(void)
{
	WindowContext	*ctx = window_init(640, 480, "X11 Engine Split");
	if (!ctx) return (1);

	int	running = 1;

	while (running)
		window_poll(ctx, &running);

	window_cleanup(ctx);

	return (0);
}

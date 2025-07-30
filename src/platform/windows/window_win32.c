#include "window_manager.h"

#include <stdlib.h>

#ifdef _WIN32

LRESULT CALLBACK	WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return (0);
	}

	return (DefWindowProc(hwnd, msg, wParam, lParam);
}

WindowContext	*window_init(int width, int height, const char *title)
{
	WindowContext	*ctx;

	ctx = malloc(sizeof(WindowContext));
	if (!ctx) return (NULL);

	ctx->width = width;
	ctx->height = height;

	ctx->hinstance = GetModuleHandle(NULL);

	ctx->wndclass = (WNDCLASS) {
		.lpfnWndProc = WndProc,
		.hInstance = ctx->hinstance,
		.lpszClassName = "MyAppWindowClass"
	};

	if (!RegisterClass(&ctx->wndclass))
	{
		free(ctx);
		return (NULL);
	}

	ctx->hwnd = CreateWindowEx(
		0,
		ctx->wndclass.lpszClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL, NULL,
		ctx->hinstance,
		NULL
	);

	if (!ctx->hwnd)
	{
		free(ctx);
		return (NULL);
	}

	ShowWindow(ctx->hwnd, SW_SHOW);
	UpdateWindow(ctx->hwnd);

	return (ctx);
}


void	window_poll(WindowContext *ctx, int *running)
{
	MSG	msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			*running = 0;
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void	window_cleanup(WindowContext *ctx)
{
	if (!ctx) return ;

	DestroyWindow(ctx->hwnd);
	UnregisterClass(ctx->wndclass.lpszClassName, ctx->hinstance);
	free(ctx);
}

#endif

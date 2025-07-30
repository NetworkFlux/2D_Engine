#ifdef __APPLE__
#import "window_manager.h"
#import <Cocoa/Cocoa.h>

@interface WindowDelegate : NSObject <NSApplicationDelegate>
@end

@implementation WindowDelegate
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
	return YES;
}
@end

WindowContext	*window_init(int width, int height, const char *title)
{
	@autoreleasepool {
		WindowContext *ctx = malloc(sizeof(WindowContext));
		if (!ctx)
			return NULL;

		NSApplication *app = [NSApplication sharedApplication];
		[app setActivationPolicy:NSApplicationActivationPolicyRegular];

		ctx->delegate = [WindowDelegate new];
		[app setDelegate:(id<NSApplicationDelegate>)ctx->delegate];

		NSString *nsTitle = [NSString stringWithUTF8String:title];
		NSRect frame = NSMakeRect(0, 0, width, height);
		NSWindowStyleMask style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

		ctx->window = [[NSWindow alloc] initWithContentRect:frame
												   styleMask:style
													 backing:NSBackingStoreBuffered
													   defer:NO];
		[ctx->window setTitle:nsTitle];
		[ctx->window makeKeyAndOrderFront:nil];
		[NSApp activateIgnoringOtherApps:YES];

		ctx->running = 1;
		return ctx;
	}
}

void window_poll(WindowContext *ctx, int *running)
{
	@autoreleasepool {
		NSEvent *event;
		while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
											untilDate:[NSDate distantPast]
											   inMode:NSDefaultRunLoopMode
											  dequeue:YES])) {
			[NSApp sendEvent:event];
			[NSApp updateWindows];

			if ([event type] == NSEventTypeKeyDown || [event type] == NSEventTypeApplicationDefined) {
				*running = 0;
			}
		}
	}
}

void window_cleanup(WindowContext *ctx)
{
	@autoreleasepool {
		[ctx->window close];
		[ctx->delegate release];
		free(ctx);
	}
}
#endif

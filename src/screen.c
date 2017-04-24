#include "screen.h"
#include "os.h"

#if defined(IS_MACOSX)
	#include <ApplicationServices/ApplicationServices.h>
	#include <QuartzCore/QuartzCore.h>
#elif defined(USE_X11)
	#include <X11/Xlib.h>
	#include "xdisplay.h"
#endif

MMSize getMainDisplaySize(void)
{
#if defined(IS_MACOSX)
	CGDirectDisplayID displayID = CGMainDisplayID();
	return MMSizeMake(CGDisplayPixelsWide(displayID),
	                  CGDisplayPixelsHigh(displayID));
#elif defined(USE_X11)
	Display *display = XGetMainDisplay();
	const int screen = DefaultScreen(display);

	return MMSizeMake((size_t)DisplayWidth(display, screen),
	                  (size_t)DisplayHeight(display, screen));
#elif defined(IS_WINDOWS)
	return MMSizeMake((size_t)GetSystemMetrics(SM_CXSCREEN),
	                  (size_t)GetSystemMetrics(SM_CYSCREEN));
#endif
}

MMRect getWindowRect(int windowID)
{
#if defined(IS_MACOSX)
	CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);
	CFIndex i;

	for (i = 0; i < CFArrayGetCount(windowList); ++i) {
		CFDictionaryRef win = CFArrayGetValueAtIndex(windowList, i);

		int id;
		CFNumberGetValue(CFDictionaryGetValue(win, kCGWindowNumber), kCFNumberIntType, &id);

		if (id == windowID) {
			CFDictionaryRef boundsRef = CFDictionaryGetValue(win, kCGWindowBounds);
			CGRect bounds;
			CGRectMakeWithDictionaryRepresentation(boundsRef, &bounds);
			return MMRectMake(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
		}
	}
#endif
	return MMRectMake(0, 0, 0, 0);
}

bool pointVisibleOnMainDisplay(MMPoint point)
{
	MMSize displaySize = getMainDisplaySize();
	return point.x < displaySize.width && point.y < displaySize.height;
}

#include "window.h"
#include "os.h"

#if defined(IS_MACOSX)
    #include <ApplicationServices/ApplicationServices.h>
    #include <QuartzCore/QuartzCore.h>
#endif

pid_t getFocusedProcessID() {
    #if defined(IS_MACOSX)
        ProcessSerialNumber processSerialNumber;
        GetFrontProcess(&processSerialNumber);
        pid_t frontProcessID;
        GetProcessPID(&processSerialNumber, &frontProcessID);
        return frontProcessID;
    #endif

    return 0;
}

std::vector<MMWindow> getWindowList() {
    pid_t focusedPID = getFocusedProcessID();

    CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);
    CFIndex windowCount = CFArrayGetCount(windowList);

    std::vector<MMWindow> retVal(windowCount);

    for (CFIndex i = 0; i < windowCount; ++i) {
        CFDictionaryRef windowRef = (CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);

        CFNumberRef windowOwnerPIDRef = (CFNumberRef)CFDictionaryGetValue(windowRef, kCGWindowOwnerPID);
        CFNumberRef windowNumberRef = (CFNumberRef)CFDictionaryGetValue(windowRef, kCGWindowNumber);
        CFStringRef windowNameRef = (CFStringRef)CFDictionaryGetValue(windowRef, kCGWindowName);
        CFDictionaryRef windowBoundsRef = (CFDictionaryRef)CFDictionaryGetValue(windowRef, kCGWindowBounds);

        MMWindow window;
        CFNumberGetValue(windowOwnerPIDRef, kCFNumberIntType, &window.pid);
        CFNumberGetValue(windowNumberRef, kCFNumberIntType, &window.number);
        window.name = CFStringGetCStringPtr(windowNameRef, kCFStringEncodingUTF8);
        CGRect windowBounds;
        CGRectMakeWithDictionaryRepresentation(windowBoundsRef, &windowBounds);
        window.bounds = MMRectMake(windowBounds.origin.x, windowBounds.origin.y, windowBounds.size.width, windowBounds.size.height);
        window.ownerIsFocused = (pid_t)window.pid == focusedPID;

        retVal.push_back(window);
    }

    CFRelease(windowList);

    return retVal;
}

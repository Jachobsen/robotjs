#include "window.h"
#include "os.h"
#include "process.h"
#include <string>

#if defined(IS_MACOSX)
    #include <QuartzCore/QuartzCore.h>

    bool ToUtf8(const CFStringRef str16, std::string* str8) {
        if ((nullptr == str16) || (nullptr == str8)) {
            return false;
        }
        size_t maxlen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(str16),
                                                        kCFStringEncodingUTF8) + 1;
        std::unique_ptr<char[]> buffer(new char[maxlen]);
        if (!buffer || !CFStringGetCString(str16, buffer.get(), maxlen,
                                         kCFStringEncodingUTF8)) {
            return false;
        }
        str8->assign(buffer.get());
        return true;
    }
#endif

std::vector<MMWindow> getWindowList() {
    int focusedPID = getFocusedProcess();

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
        CGRect windowBounds;
        CGRectMakeWithDictionaryRepresentation(windowBoundsRef, &windowBounds);
        window.bounds = MMRectMake(windowBounds.origin.x, windowBounds.origin.y, windowBounds.size.width, windowBounds.size.height);
        window.ownerIsFocused = (int)window.pid == focusedPID;

        std::string windowName;
        if (!ToUtf8(windowNameRef, &(windowName))) {
            continue;
        }
        window.name = strcpy((char*)malloc(windowName.length()+1), windowName.c_str());;

        retVal.push_back(window);
    }

    CFRelease(windowList);

    return retVal;
}

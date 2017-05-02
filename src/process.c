#include "os.h"
#include "process.h"

#if defined(IS_MACOSX)
    #include <ApplicationServices/ApplicationServices.h>
#endif

int getFocusedProcess() {
    #if defined(IS_MACOSX)
        ProcessSerialNumber processSerialNumber;
        GetFrontProcess(&processSerialNumber);
        int processID;
        GetProcessPID(&processSerialNumber, &processID);
        return processID;
    #endif

    return 0;
}

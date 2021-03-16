#include "Waitable timer.h"

int _tmain() {

    // declare our local variables
    HANDLE hTimer;
    SYSTEMTIME st;
    FILETIME ftLocal, ftUTC;
    LARGE_INTEGER liUTC;

    // craete an auto-reset timer
    hTimer = CreateWaitableTimer(NULL, FALSE, NULL);


    return 0;
}
#include "_Context.h"


int _tmain() {

    //// Create a CONTEXT struceure,
    //CONTEXT _Context;
    //// Tell the system that we are instreasted in all
    //_Context.ContextFlags = CONTEXT_FULL;

    //SuspendThread(GetCurrentThread());
    //GetThreadContext((HANDLE)GetCurrentThreadId(), &_Context);
    //ResumeThread((HANDLE)GetCurrentThreadId());
    //return 0;
    ULONG_PTR lpProcessAffinityMask;
    ULONG_PTR lpSystemAffinityMask;
    GetProcessAffinityMask(GetCurrentProcess(), &lpProcessAffinityMask, &lpSystemAffinityMask);
    return 0;
}
#pragma once
#pragma warning(disable : 26495)

#include "../CommonFiles/CmnHdr.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <StrSafe.h>
#include <windowsx.h>
#include "resource.h"

class CQueue
{
public:
    typedef struct _ELEMENT_ {
        int m_nThreadNum, m_nRequestNum;
        // Other element data should go here
    }ELEMENT, * PELEMENT;

private:
    PELEMENT m_pElements;        // Array of elements to be processed
    int      m_nMaxElements;     // Maximum # of elements in the array
    HANDLE   m_h[2];             // Mutex & semaphore handles
    HANDLE& m_hmtxQ;           // Reference to m_h[0]
    HANDLE& m_hsemNumElements; // Reference to m_h[1]

public:
    CQueue(int nMaxElements);
    ~CQueue();

    BOOL Append(PELEMENT pElement, DWORD dwMilliseconds);
    BOOL Remove(PELEMENT pElement, DWORD dwMilliseconds);
};


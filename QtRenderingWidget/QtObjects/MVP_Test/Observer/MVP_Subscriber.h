#pragma once
#include <windows.h>

class MVPViewSubscriber {
public:
	virtual void NotifyResize(UINT width, UINT height) = 0;
	virtual LRESULT WINAPI NotifyNativeWindowProcedure(MSG* pMsg) = 0;
};
#pragma once


// -- forward declaration --
#include "d3d11.h"
/// <summary>
/// Interface for buffer that needs to be resized when the window changes size
/// </summary>
class IUpdateable
{
public:
	virtual void Update(float timeElapsed) = 0;
};


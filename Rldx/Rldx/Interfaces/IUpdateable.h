#pragma once

/// <summary>
/// Interface for buffer that needs to be resized when the window changes size
/// </summary>
class IUpdateable
{
public:
	virtual void Update(float timeElapsed = 0.0f) = 0;
};


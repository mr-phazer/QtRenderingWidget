#pragma once

/// <summary>
/// Interface class, for game objects that needs to be updated every frame.
/// could be used for geomtry objects, animations, etc.
/// </summary>
class IUpdateable
{
public:
	virtual void Update(float timeElapsed = 0.0f) = 0;
};
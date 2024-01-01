#pragma once


// -- forward declaration --
class ID3D11Device;
class ID3D11DeviceContext;

/// <summary>
/// Interface for buffer that needs to be resized when the window changes size
/// </summary>
class IUpdateable
{
public:
	virtual void Update(float timeElapsed) = 0;
};


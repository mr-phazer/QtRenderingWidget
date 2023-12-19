#pragma once


/// <summary>
/// Interface for buffer that needs to be resized when the window changes size
/// </summary>
class IResizable
{
public:
	virtual void Reset(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) = 0;
};


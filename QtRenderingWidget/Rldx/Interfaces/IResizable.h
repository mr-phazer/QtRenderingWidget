#pragma once


namespace Rldx 
{
	/// <summary>
	/// Interface for buffer that needs to be resized when the window changes size
	/// </summary>
	class IResizable
	{
	public:
		virtual void Resize(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) = 0;
	};

}; // namespace Rldx
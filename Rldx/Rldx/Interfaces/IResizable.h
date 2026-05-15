#pragma once


namespace rldx 
{
	/// <summary>
	/// Interface for d3d11 resizable 2d resurce
	/// </summary>
	class IResizable
	{
	public:
		virtual void Resize(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) = 0;
	};

}; // namespace rldx
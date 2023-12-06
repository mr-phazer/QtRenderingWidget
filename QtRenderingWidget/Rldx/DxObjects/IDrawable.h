#pragma once

class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11RenderTargetView;

namespace Rldx {
		
	class IDxShaderProgram;

	enum class ItemTypeEnum : int
	{
		Mesh
	};

	/// <summary>
	/// Base for anything that be draw with a shader program
	/// </summary>
	class IDrawable
	{
	public:
		virtual void Draw(ID3D11DeviceContext* poDC, ID3D11RenderTargetView* destRtV = nullptr) = 0;

		//ItemTypeEnum virtual GetNodeType() = 0;
	};

	/// <summary>
	/// Interface for buffer that needs to be resized when the window changes size
	/// </summary>
	class IResizable
	{
	public:
		virtual void Reset(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) = 0;
	};
}
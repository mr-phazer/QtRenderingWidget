#pragma once

class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11RenderTargetView;

namespace Rldx {
	
	/// <summary>
	/// Base for anything that be draw with a shader program
	/// </summary>
	class IDrawable
	{
	public:
		virtual void Draw(ID3D11DeviceContext* poDC) = 0;		
	};
};
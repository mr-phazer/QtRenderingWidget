#pragma once

class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11RenderTargetView;

namespace Rldx {
	
	/// <summary>
	/// Interface for objects that can be drawn on/with a D3d11 device context
	/// </summary>
	class IDrawable
	{
	public:		
		virtual void Draw(ID3D11DeviceContext* poDC) = 0;		
	};
};
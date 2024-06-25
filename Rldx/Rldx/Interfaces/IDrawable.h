#pragma once

#include <d3d11.h>

namespace rldx {

	/// <summary>
	/// Interface for objects that can be drawn on/with a D3d11 device context
	/// </summary>
	class IDrawable
	{
	public:
		virtual void Draw(ID3D11DeviceContext* poDC) = 0;
	};
};
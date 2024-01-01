#pragma once

class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11RenderTargetView;

namespace Rldx {

	/// <summary>
	/// Base for anything the be bound to the pipeline
	/// </summary>
	class IBindable	
	{
	public:
		virtual void BindToDC(ID3D11DeviceContext* poDC) = 0;
	};
};

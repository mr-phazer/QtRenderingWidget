#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

namespace rldx {

	/// <summary>
	/// Base for anything the be bound to the pipeline
	/// </summary>
	class IBindable
	{
	public:
		virtual void BindToDC(ID3D11DeviceContext* poDC) = 0;
		virtual void UnbindFromDC(ID3D11DeviceContext* poDC) {};
		virtual uint32_t GetStartSlot() { return 0; }
	};
};

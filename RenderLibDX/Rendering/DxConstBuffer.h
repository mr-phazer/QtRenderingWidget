#pragma once

#pragma once

#include <DirectXMath.h>
#include <SimpleMath.h>

#include "..\..\DirectXTK\Inc\BufferHelpers.h"

namespace rldx {

	template<typename DATA_TYPE>
	struct TConstBuffer
	{
		/// <summary>
		/// data, typically a struct, to be copied to the GPU
		/// Stored on and editable from the CPU
		/// </summary>
		DATA_TYPE data;

		/// <summary>
		/// Wraps GPU data buffer (MS DirectXTK class
		/// </summary>
		DirectX::ConstantBuffer<DATA_TYPE> buffer; 

		void RefreshGPUData(ID3D11DeviceContext* poDeviceContext)
		{
			buffer.SetData(poDeviceContext, data);
		}
	};	

	/// <summary>
	/// Vertex shader constant buffer
	/// </summary>
	template<typename DATA_TYPE>
	struct TDxVSShaderConstBuffer : public TConstBuffer<DATA_TYPE>
	{	
		void BindToDC(ID3D11DeviceContext* poDeviceContext)
		{
			RefreshGPUData(poDeviceContext);
			ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { buffer.GetBuffer() };
			poDeviceContext->VSSetConstantBuffers(0, 1, vertexShaderSceneConstBuffers);
		}
	};	

	/// <summary>
	/// Pixel shader constant buffer
	/// </summary>
	template<typename DATA_TYPE, UINT T_startSlot = 0>
	struct TDxPSShaderConstBuffer : public TConstBuffer<DATA_TYPE>, public IBindable
	{	
		UINT m_startSlot = T_startSlot;
	public:
		void SetStartSlot(UINT slot)
		{
			m_startSlot = slot;
		}

		void BindToDC(ID3D11DeviceContext* poDeviceContext) override
		{
			RefreshGPUData(poDeviceContext);
			ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { buffer.GetBuffer() };
			poDeviceContext->PSSetConstantBuffers(m_startSlot, 1, vertexShaderSceneConstBuffers);
		}
	};	
};


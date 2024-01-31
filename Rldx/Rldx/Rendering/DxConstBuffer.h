#pragma once

#include "..\..\DirectXTK\Inc\BufferHelpers.h"
#include "..\Interfaces\IBindable.h"

#include <d3d11.h>

namespace rldx {

	template<typename CONST_BUF_DATA_TYPE>
	struct TConstBuffer
	{
	public:
		/// <summary>
		/// data, typically a struct, to be copied to the GPU
		/// Stored on and editable from the CPU
		/// </summary>
		CONST_BUF_DATA_TYPE data;

		/// <summary>
		/// Wraps GPU data buffer (MS DirectXTK class
		/// </summary>
		DirectX::ConstantBuffer<CONST_BUF_DATA_TYPE> buffer; 	

		/// <summary>
		/// Copiesw the internal data to the CPU
		/// </summary>
		/// <param name="poDeviceContext"></param>
		virtual void RefreshGPUData(ID3D11DeviceContext* poDeviceContext)
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

		UINT m_startSlot = 0;

	public:
		void SetStartSlot(UINT slot)
		{
			m_startSlot = slot;
		}

		virtual void BindToDC(ID3D11DeviceContext* poDeviceContext)
		{
			TConstBuffer<DATA_TYPE>::RefreshGPUData(poDeviceContext);
			ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { TConstBuffer<DATA_TYPE>::buffer.GetBuffer() };
			poDeviceContext->VSSetConstantBuffers(m_startSlot, 1, vertexShaderSceneConstBuffers);
		}
	};	

	/// <summary>
	/// Pixel shader constant buffer
	/// </summary>
	template<typename DATA_TYPE>
	struct TDxPSShaderConstBuffer : public TConstBuffer<DATA_TYPE>, public IBindable
	{	
		UINT m_startSlot = 0;

	public:
		void SetStartSlot(UINT slot)
		{
			m_startSlot = slot;
		}

		void BindToDC(ID3D11DeviceContext* poDeviceContext) override
		{						
			this->RefreshGPUData(poDeviceContext);
			ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { this->buffer.GetBuffer() };
			poDeviceContext->PSSetConstantBuffers(m_startSlot, 1, vertexShaderSceneConstBuffers);
		}
	};	
};


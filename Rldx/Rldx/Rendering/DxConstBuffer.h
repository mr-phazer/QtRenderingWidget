#pragma once

#include <memory>
#include "..\..\DirectXTK\Inc\BufferHelpers.h"
#include "..\..\DXUT\Core\DXUT.h"
#include "..\Interfaces\IBindable.h"

#include <d3d11.h>
#include <string>

namespace rldx {

	/// <summary>
	/// Wrapper for MS Direct::ConstantBuffer
	/// Which stores the CPU side data 
	/// </summary>
	/// <typeparam m_nodeName="CONST_BUF_DATA_TYPE">CPU side constant buffer type, typically a struct</typeparam>
	template<typename CONST_BUF_DATA_TYPE>
	class TConstBuffer
	{
		/// <summary>
		/// Wraps GPU data buffer (MS DirectXTK class
		/// </summary>
		std::shared_ptr<DirectX::ConstantBuffer<CONST_BUF_DATA_TYPE>> buffer;

	public:
		// TOD: make this pointer?
			/// <summary>
			/// data, typically a struct, to be copied to the GPU
			/// Stored on and editable from the CPU
			/// </summary>		/
		CONST_BUF_DATA_TYPE data;

	public:
		void Init(ID3D11Device* poDevice, std::string debugName)
		{
			buffer = std::make_shared<DirectX::ConstantBuffer<CONST_BUF_DATA_TYPE>>();
			buffer->Create(poDevice);
			DXUT_SetDebugName(buffer->GetBuffer(), debugName.c_str());
		}

		// Looks up the underlying D3D constant buffer.
		ID3D11Buffer* GetBuffer() const noexcept { return buffer->GetBuffer(); }

		/// <summary>
		/// Copiesw the internal data to the CPU
		/// </summary>
		/// <param m_nodeName="poDeviceContext"></param>
		virtual void RefreshGPUData(ID3D11DeviceContext* poDeviceContext)
		{
			buffer->SetData(poDeviceContext, data);
		}

		virtual void RefreshGPUData(ID3D11DeviceContext* poDeviceContext, const CONST_BUF_DATA_TYPE& value)
		{
			buffer->SetData(poDeviceContext, value);
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
			ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { TConstBuffer<DATA_TYPE>::GetBuffer() };
			poDeviceContext->VSSetConstantBuffers(m_startSlot, 1, vertexShaderSceneConstBuffers);
		}

		void CopyAndBindToDC(ID3D11DeviceContext* poDeviceContext, const DATA_TYPE& inData)
		{
			TConstBuffer<DATA_TYPE>::RefreshGPUData(poDeviceContext, inData);
			ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { TConstBuffer<DATA_TYPE>::GetBuffer() };
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
			TConstBuffer<DATA_TYPE>::RefreshGPUData(poDeviceContext);
			ID3D11Buffer* pixelShaderSceneConstBuffers[1] = { TConstBuffer<DATA_TYPE>::GetBuffer() };
			poDeviceContext->PSSetConstantBuffers(m_startSlot, 1, pixelShaderSceneConstBuffers);
		}
	};
};


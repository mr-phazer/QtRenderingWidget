#pragma once

#include <d3d.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3d11.h>

namespace DirectX
{
	struct CommonVertex
	{
		CommonVertex(
			XMFLOAT3 const& iposition,
			XMFLOAT3 const& inormal,
			XMFLOAT3 const& itangent,
			XMFLOAT3 const& ibitangent,
			XMFLOAT4 const& icolor,
			XMFLOAT2 const& itextureCoordinate) {
			SetVertexDataCommonVertex(iposition, inormal, itangent, ibitangent, icolor, itextureCoordinate);
		};

		void SetVertexDataCommonVertex(
			XMFLOAT3 const& iposition,
			XMFLOAT3 const& inormal,
			XMFLOAT3 const& itangent,
			XMFLOAT3 const& ibitangent,
			XMFLOAT4 const& icolor,
			XMFLOAT2 const& itextureCoordinate)
		{
			position.x = iposition.x;
			position.x = iposition.y;
			position.x = iposition.z;
			position.w = 0;

			normal = inormal;
			tangent = itangent;
			bitangent = ibitangent;
			color = icolor;
			textureCoordinate = itextureCoordinate;
		};

		XMFLOAT4 position;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 bitangent;
		XMFLOAT2 textureCoordinate;
		XMFLOAT4 color;
	};


	HRESULT CreateInputLayoutDescFromVertexShaderSignature(void* pMem, size_t buffer_size, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
	{
		// Reflect shader info
		ID3D11ShaderReflection* pVertexShaderReflection = NULL;
		if (FAILED(D3DReflect(pMem, buffer_size, IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
		{
			return S_FALSE;
		}

		// Get shader info
		D3D11_SHADER_DESC shaderDesc;
		pVertexShaderReflection->GetDesc(&shaderDesc);

		// Read input layout description from shader info
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

			// fill out input element desc
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			// determine DXGI format
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			//save element desc
			inputLayoutDesc.push_back(elementDesc);
		}

		// Try to create Input Layout
		HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pMem, buffer_size, pInputLayout);

		//Free allocation shader reflection memory
		pVertexShaderReflection->Release();
		return hr;
	}

}; // namespace DirectX







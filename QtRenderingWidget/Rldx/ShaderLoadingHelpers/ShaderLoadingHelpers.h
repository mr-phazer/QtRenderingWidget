#pragma once

// STL
#include <string>
#include <vector>

// DX3d
#include <d3d11.h>
#include <d3dcompiler.h>

// Author
#include "..\DataTypes\ShaderFiles.h"

namespace Rldx {

	class ShaderLoaderHelper
	{
	public:
		static std::vector<uint8_t> GetRawDataFromDisk(const std::wstring& shaderPath);
	};

	class PixelShaderLoader
	{
	public:
		static PixelShaderFile CreatePixelShaderFromDisk(ID3D11Device* poDevice, const std::wstring& wstrPath)
		{
			ID3D11PixelShader* pDestShader = nullptr;;

			auto shaderCodeRaw = ShaderLoaderHelper::GetRawDataFromDisk(wstrPath);

			HRESULT hr = poDevice->CreatePixelShader(
				shaderCodeRaw.data(),
				shaderCodeRaw.size(),
				nullptr,
				&pDestShader
			);


			return PixelShaderFile(pDestShader);
		};

		static PixelShaderFile CreatePixelShaderFromMemory(ID3D11Device* poDevice, uint8_t* pSource, size_t sizeInBytes)
		{
			ID3D11PixelShader* pDestShader = nullptr;;
			HRESULT hr = poDevice->CreatePixelShader(pSource, sizeInBytes, nullptr, &pDestShader);

			return PixelShaderFile(pDestShader);
		};
	};

	class VertexShaderLoader
	{
	public:
		static VertexShaderFile CreateVertexShaderFromDisk(ID3D11Device* poDevice, const std::wstring& wstrPath)
		{
			ID3D11VertexShader* pDestShader = nullptr;;
			auto shaderCodeRaw = ShaderLoaderHelper::GetRawDataFromDisk(wstrPath);

			HRESULT hr = poDevice->CreateVertexShader(
				shaderCodeRaw.data(),
				shaderCodeRaw.size(),
				nullptr,
				&pDestShader
			);

			// -- make input layout from vertex signature in shader code
 			auto poInputLayout = CreateInputLayoutDescFromVertexShaderSignature(poDevice, shaderCodeRaw.data(), shaderCodeRaw.size());

			return (poInputLayout) ? VertexShaderFile(pDestShader, poInputLayout) : VertexShaderFile(pDestShader);			
		};

		static VertexShaderFile CreateVertexShaderFromMemory(ID3D11Device* poDevice, uint8_t* pSource, size_t sizeInBytes)
		{
			ID3D11VertexShader* pDestShader = nullptr;;
			HRESULT hr = poDevice->CreateVertexShader(pSource, sizeInBytes, nullptr, &pDestShader);

			return VertexShaderFile(pDestShader);
		};


		static ID3D11InputLayout* CreateInputLayoutDescFromVertexShaderSignature(
			ID3D11Device* pD3DDevice,
			void* pMem,
			size_t buffer_size
			)
		{
			// Reflect shader info
			ID3D11ShaderReflection* pVertexShaderReflection = NULL;
			if (FAILED(D3DReflect(pMem, buffer_size, IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
			{
				return nullptr;
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
			ID3D11InputLayout* pInputLayout = NULL;
			HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pMem, buffer_size, &pInputLayout);

			//Free allocation shader reflection memory
			pVertexShaderReflection->Release();

			if (FAILED(hr)) {
				return nullptr;
			}

			return pInputLayout;
		};

	}; // class VertexShaderLoader
}; // namespace Rldx
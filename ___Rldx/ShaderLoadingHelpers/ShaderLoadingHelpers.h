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
		);

	}; // class VertexShaderLoader
}; // namespace Rldx
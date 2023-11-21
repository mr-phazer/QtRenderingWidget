#pragma once


//  WRL wrappers from COM Objects - smart pointers for Direct3d resources
#include <wrl/client.h>
#include <d3d11.h>
#include <string>
#include <memory>
#include <vector>

#include "..\..\Common\TSmartPointer.h"
#include "..\..\Tools\tools.h"

namespace Rldx {


	enum class ShaderTypeEnum
	{
		Vertex = 0,
		Pixel = 1,
		Geometry = 2
	};

	class VertexShaderFile
	{
	public:
		void SetShader(ID3D11VertexShader* poShaderCode) {
			m_cpoShaderCode = poShaderCode;
		};

		ID3D11VertexShader* GSetShader() {
			return m_cpoShaderCode.Get();
		};

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_cpoShaderCode;
	};

	class PixelShaderFile
	{
	public:
		PixelShaderFile(ID3D11PixelShader* poShaderCode) : m_cpoShaderCode(poShaderCode) {};

		void SetShader(ID3D11PixelShader* poShaderCode) {
			m_cpoShaderCode = Microsoft::WRL::ComPtr<ID3D11PixelShader>(poShaderCode);
		};

		ID3D11PixelShader* GSetShader() {
			return m_cpoShaderCode.Get();
		};

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_cpoShaderCode;
	};

	class ShaderFactoryHelper
	{
	public:
		static std::vector<uint8_t> GetRawDataFromDisk(const std::wstring& shaderPath)
		{
			size_t shaderCodeSize = tools::getFileSize(shaderPath);
			std::vector<uint8_t> shaderCodeRaw(shaderCodeSize);
			std::ifstream in(shaderPath, std::ios::binary);
			in.read((char*)shaderCodeRaw.data(), shaderCodeSize);
			in.close();

			return shaderCodeRaw;
		}
				
	};

	class ShaderLoader
	{
		
	public:
		static PixelShaderFile CreatePixelShaderFromDisk(ID3D11Device* poDevice, const std::wstring& wstrPath)
		{			
			ID3D11PixelShader* pDestShader = nullptr;;

			auto shaderCodeRaw = ShaderFactoryHelper::GetRawDataFromDisk(wstrPath);

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


#include "..\..\..\DirectXTK\Inc\BufferHelpers.h"

	DirectX::ConstantBuffer<PixelShaderFile> m_constantBuffer;


	class DxShaderProgram
	{
	public:

		void init(ID3D11Device* device, std::wstring pixelShaderPath,  std::wstring vertexShaderPath)
		{

			//if (!vertexShaderPath.empty())
			//m_VertexShaderFile = ShaderFactory::CreateShaderFromDisk<ID3D11VertexShader>(p, vertexShaderPath);

			if (!pixelShaderPath.empty())
				m_pixelShaderFile = ShaderLoader::CreatePixelShaderFromDisk(device, pixelShaderPath);				
		}



	private:
		PixelShaderFile m_pixelShaderFile;
		//VertexShaderFile m_VertexxlShaderFile;
	};


}; // namespace Rldx


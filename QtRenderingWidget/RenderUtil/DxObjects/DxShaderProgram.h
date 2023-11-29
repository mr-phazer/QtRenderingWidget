#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <string>
#include <memory>
#include <vector>

// MS Direct3d lib
#include "..\..\..\DirectXTK\Inc\BufferHelpers.h"

// author
#include "..\..\Common\TSmartPointer.h"
#include "..\..\Tools\tools.h"
#include "..\ConstBuffers\ConstBuffers.h"

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
		VertexShaderFile(ID3D11VertexShader* poShaderCode) : m_cpoShaderCode(poShaderCode) {};
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

	class PixelShaderLoader
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

	class VertexShaderLoader
	{
	public:
		VertexShaderFile CreateVertexShaderFromDisk(ID3D11Device* poDevice, const std::wstring& wstrPath)
		{
			ID3D11VertexShader* pDestShader = nullptr;;
			auto shaderCodeRaw = ShaderFactoryHelper::GetRawDataFromDisk(wstrPath);

			HRESULT hr = poDevice->CreateVertexShader(
				shaderCodeRaw.data(),
				shaderCodeRaw.size(),
				nullptr,
				&pDestShader
			);						

			return VertexShaderFile(pDestShader);
		};

		VertexShaderFile CreateVertexShaderFromMemory(ID3D11Device* poDevice, uint8_t* pSource, size_t sizeInBytes)
		{
			ID3D11VertexShader* pDestShader = nullptr;;
			HRESULT hr = poDevice->CreateVertexShader(pSource, sizeInBytes, nullptr, &pDestShader);

			return VertexShaderFile(pDestShader);
		};

	}; 	


	class IDxShaderProgram
	{
	public:
		virtual PixelShaderFile* GetPixelShaderFile() = 0;
		virtual VertexShaderFile* GetVertexShaderFile() = 0;

		virtual ID3D11Buffer* GetPixelShaderConstBuffer() const = 0;
		virtual ID3D11Buffer* GetVertexShaderConstBuffer() const = 0;
	};

	template <typename PS_CONST_BUFER, typename VS_CONST_BUFER>
	class DxShaderProgram : public IDxShaderProgram
	{	
		PixelShaderFile m_pixelShaderFile;
		VertexShaderFile m_vertexShaderFile;
		DirectX::ConstantBuffer<VS_CONST_BUFER> m_vertexShaderConstBuffer;
		DirectX::ConstantBuffer<PS_CONST_BUFER> m_pixelShaderConstBuffer;

	public:
		virtual void Create(ID3D11Device* poDevice, std::pair<void*, size_t> vertexShaderMem, std::pair<void*, size_t> pixelShaderMem)
		{
			if (!vertexShaderMem.first != nullptr && vertexShaderMem.second > 0)
				m_VertexShaderFile = PixelShaderLoader::CreatePixelShaderFromMemory(poDevice, vertexShaderMem.first, vertexShaderMem.second);

			if (!pixelShaderMem.first != nullptr && pixelShaderMem.second > 0)
				m_pixelShaderFile = VertexShaderLoader::CreatePixelShaderFromMemory(poDevice, pixelShaderMem.first, pixelShaderMem.second);

			m_pixelShaderConstBuffer.Create(poDevice);
			m_vertexShaderConstBuffer.Create(poDevice);
		}

		virtual void Create(ID3D11Device* poDevice, std::wstring vertexShaderPath, std::wstring pixelShaderPath)
		{			
			if (!vertexShaderPath.empty())
				m_VertexShaderFile = PixelShaderLoader::CreatePixelShaderFromDisk(poDevice, vertexShaderPath);						

			if (!pixelShaderPath.empty())
				m_pixelShaderFile = VertexShaderLoader::CreateVertexShaderFromDisk(poDevice, pixelShaderPath);				

			m_pixelShaderConstBuffer.Create(poDevice);
			m_vertexShaderConstBuffer.Create(poDevice);				
		}
		
		PixelShaderFile* GetPixelShaderFile() { return &m_pixelShaderFile; };
		VertexShaderFile* GetVertexShaderFile() { return &m_VertexShaderFile;};

		ID3D11Buffer* GetPixelShaderConstBuffer() const { return m_pixelShaderConstBuffer.GetBuffer(); };
		ID3D11Buffer* GetVertexShaderConstBuffer() const { return m_vertexShaderConstBuffer.GetBuffer(); };

		void SetPixelShaderConstBuffer(ID3D11DeviceContext* deviceContext, PS_CONST_BUFER& data)
		{
			m_pixelShaderConstBuffer.SetData(deviceContext, data);
		};
	};

	using DxStandardShaderProgram  = DxShaderProgram<VS_MeshConstantBuffer, PS_MeshConstantBuffer>;
	
	

}; // namespace Rldx


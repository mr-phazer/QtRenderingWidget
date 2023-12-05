#pragma once


#include <d3d11.h>
#include <string>
#include <vector>

// MS Direct3d lib
#include "..\..\..\DirectXTK\Inc\BufferHelpers.h"

// author
#include "..\..\Tools\tools.h"
#include "..\Types\ConstBuffers\CPUConstBuffers.h"
#include "..\Types\ShaderFiles.h"

namespace Rldx {

	// TODO: remove
	//enum class ShaderTypeEnum
	//{
	//	Vertex = 0,
	//	Pixel = 1,
	//	Geometry = 2
	//};


	class IDxShaderProgram
	{
	public:
		virtual ID3D11VertexShader* GetVertexShader() = 0;
		virtual ID3D11PixelShader* GetPixelShader() = 0;

		virtual ID3D11Buffer* GetPixelShaderConstBuffer() const = 0;
		virtual ID3D11Buffer* GetVertexShaderConstBuffer() const = 0;

		virtual void UpdateVSConstBuffers(ID3D11DeviceContext*) = 0;
		virtual void UpdatePSConstBuffers(ID3D11DeviceContext*) = 0;

		virtual void GetReady(ID3D11DeviceContext*) = 0;

		//	virtual void SetVSConstBufferAsActive(ID3D11DeviceContext* poDC) const
		//	{
		//		auto pConstBufferTemp = GetVertexShaderConstBuffer();
		//		poDC->VSSetConstantBuffers(0, 1, &pConstBufferTemp);
		//	};

		//	virtual void SetPSConstBufferAsActive(ID3D11DeviceContext* poDC) const
		//	{
		//		ID3D11Buffer* pConstBufferTemp[] = { GetPixelShaderConstBuffer() };
		//		poDC->PSSetConstantBuffers(0, 1, pConstBufferTemp);
		//	}

		//	virtual void SetVertexShaderAsActive(ID3D11DeviceContext* poDC) const
		//	{
		//		poDC->PSSetShader(GetPixelShader(), nullptr, 0);
		//	}

		//	virtual void SetPixelShaderSAsActive(ID3D11DeviceContext* poDC) const
		//	{
		//		poDC->PSSetShader(GetPixelShader(), nullptr, 0);
		//	}

		//	virtual  void SetPSConstBufferAsActive(ID3D11DeviceContext* poDC) const
		//	{
		//		auto pConstBufferTemp = GetPixelShaderConstBuffer();
		//		poDC->VSSetConstantBuffers(0, 1, &pConstBufferTemp);
		//	}
	};

	template <typename VS_CONST_BUFER, typename PS_CONST_BUFER>
	class TDxShaderProgram : public IDxShaderProgram
	{

		PixelShaderFile m_pixelShaderFile;
		VertexShaderFile m_vertexShaderFile;
		DirectX::ConstantBuffer<VS_CONST_BUFER> m_vertexShaderConstBuffer;
		DirectX::ConstantBuffer<PS_CONST_BUFER> m_pixelShaderConstBuffer;

		VS_CONST_BUFER vertexShaderConstBuffer;
		PS_CONST_BUFER pixelShaderConstBuffer;

	public:
		virtual void Create(ID3D11Device* poDevice, std::pair<uint8_t*, size_t> vertexShaderMem, std::pair<uint8_t*, size_t> pixelShaderMem)
		{
			if (vertexShaderMem.first != nullptr && vertexShaderMem.second > 0)
				m_vertexShaderFile = VertexShaderLoader::CreateVertexShaderFromMemory(poDevice, vertexShaderMem.first, vertexShaderMem.second);

			if (pixelShaderMem.first != nullptr && pixelShaderMem.second > 0)
				m_pixelShaderFile = PixelShaderLoader::CreatePixelShaderFromMemory(poDevice, pixelShaderMem.first, pixelShaderMem.second);

			m_pixelShaderConstBuffer.Create(poDevice);
			m_vertexShaderConstBuffer.Create(poDevice);
		}

		virtual void Create(ID3D11Device* poDevice, std::wstring vertexShaderPath, std::wstring pixelShaderPath)
		{
			if (!vertexShaderPath.empty())
				m_vertexShaderFile = VertexShaderLoader::CreateVertexShaderFromDisk(poDevice, vertexShaderPath);

			if (!pixelShaderPath.empty())
				m_pixelShaderFile = PixelShaderLoader::CreatePixelShaderFromDisk(poDevice, pixelShaderPath);

			m_pixelShaderConstBuffer.Create(poDevice);
			m_vertexShaderConstBuffer.Create(poDevice);
		}

		virtual ID3D11VertexShader* GetVertexShader()  override { return m_vertexShaderFile.GetShader(); };
		virtual ID3D11PixelShader* GetPixelShader()   override { return m_pixelShaderFile.GetShader(); };

		ID3D11Buffer* GetPixelShaderConstBuffer() const { return m_pixelShaderConstBuffer.GetBuffer(); };
		ID3D11Buffer* GetVertexShaderConstBuffer() const { return m_vertexShaderConstBuffer.GetBuffer(); };

		VS_CONST_BUFER& GetCSEditableVS() { return vertexShaderConstBuffer; };
		PS_CONST_BUFER& GetCSEditablePS() { return pixelShaderConstBuffer; };





	};

	class DxMeshShaderProgam : public TDxShaderProgram<VS_MeshConstantBuffer, PS_MeshConstantBuffer>
	{
	public:
		virtual void GetReady(ID3D11DeviceContext* dc) override
		{
			auto& vsCB = GetCSEditableVS();
			vsCB.mWorld = sm::Matrix::Identity;

			dc->PSSetShader(GetPixelShader(), nullptr, 0);
			dc->VSSetShader(GetVertexShader(), nullptr, 0);
		};

	};

}; // namespace Rldx


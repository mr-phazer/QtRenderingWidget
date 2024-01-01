#pragma once


#include <d3d11.h>
#include <string>


// MS Direct3d lib
#include "..\..\..\DirectXTK\Inc\BufferHelpers.h"

// author

#include "..\DataTypes\ConstBuffers\CPUConstBuffers.h"
#include "..\DataTypes\ShaderFiles.h"
#include "..\ShaderLoadingHelpers\ShaderLoadingHelpers.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"
#include "..\Managers\ResourceManager\IDxResouce.h"

#include "..\Interfaces\IBindable.h"

namespace Rldx {

	// TODO: remove
	//enum class ShaderTypeEnum
	//{
	//	Vertex = 0,
	//	Pixel = 1,
	//	Geometry = 2
	//};


	class IDxShaderProgram : public IDxResource, public IBindable
	{
	public:
		virtual ID3D11VertexShader* GetVertexShader() = 0;
		virtual ID3D11PixelShader* GetPixelShader() = 0;
		virtual ID3D11InputLayout* GetInputLayout() = 0;

		virtual ID3D11Buffer* GetPixelShaderConstBuffer() const = 0;
		virtual ID3D11Buffer* GetVertexShaderConstBuffer() const = 0;					

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


		// Inherited via IDxResource
		std::string GetTypeString() const override;
		ResourceTypeEnum GetType() const override;

		//	virtual  void SetPSConstBufferAsActive(ID3D11DeviceContext* poDC) const
		//	{
		//		auto pConstBufferTemp = GetPixelShaderConstBuffer();
		//		poDC->VSSetConstantBuffers(0, 1, &pConstBufferTemp);
		//	}
	};

	template <typename VS_CONST_BUFER, typename PS_CONST_BUFER>
	class TDxShaderProgram : public IDxShaderProgram
	{
		using Type = TDxShaderProgram<VS_CONST_BUFER, PS_CONST_BUFER>;
		PixelShaderFile m_pixelShaderFile;
		VertexShaderFile m_vertexShaderFile;
		DirectX::ConstantBuffer<VS_CONST_BUFER> m_vertexShaderConstBuffer;
		DirectX::ConstantBuffer<PS_CONST_BUFER> m_pixelShaderConstBuffer;

		VS_CONST_BUFER vertexShaderConstBuffer;
		PS_CONST_BUFER pixelShaderConstBuffer;

	public:
		template <typename SHADER_TYPE>
		static  SHADER_TYPE* Create(ID3D11Device* poDevice, std::pair<uint8_t*, size_t> vertexShaderMem, std::pair<uint8_t*, size_t> pixelShaderMem)
		{
			auto result = Rldx::DxResourceManager::Instance()->AllocShaderProgram<SHADER_TYPE>("shader01");
			//auto newInstance = static_cast<TDxShaderProgram*>(result.GetPtr());
			auto newInstance = result.GetPtr();

			if (vertexShaderMem.first != nullptr && vertexShaderMem.second > 0)
				newInstance->m_vertexShaderFile = VertexShaderLoader::CreateVertexShaderFromMemory(poDevice, vertexShaderMem.first, vertexShaderMem.second);

			if (pixelShaderMem.first != nullptr && pixelShaderMem.second > 0)
				newInstance->m_pixelShaderFile = PixelShaderLoader::CreatePixelShaderFromMemory(poDevice, pixelShaderMem.first, pixelShaderMem.second);

			newInstance->m_pixelShaderConstBuffer.Create(poDevice);
			newInstance->m_vertexShaderConstBuffer.Create(poDevice);

			return newInstance;
		};

		
		template <typename SHADER_TYPE>
		static SHADER_TYPE* Create(ID3D11Device* poDevice, std::wstring vertexShaderPath, std::wstring pixelShaderPath)
		{		
			auto result = Rldx::DxResourceManager::Instance()->AllocShaderProgram<SHADER_TYPE>("shader01");
			//auto newInstance = static_cast<TDxShaderProgram*>(result.GetPtr());
			auto newInstance = result.GetPtr();

			if (vertexShaderPath.empty() || pixelShaderPath.empty()) {
				return nullptr;
			}

			if (!vertexShaderPath.empty())
				newInstance->m_vertexShaderFile = VertexShaderLoader::CreateVertexShaderFromDisk(poDevice, vertexShaderPath);

			if (!pixelShaderPath.empty())
				newInstance->m_pixelShaderFile = PixelShaderLoader::CreatePixelShaderFromDisk(poDevice, pixelShaderPath);

			newInstance->m_pixelShaderConstBuffer.Create(poDevice);
			newInstance->m_vertexShaderConstBuffer.Create(poDevice);

			// TODO: remove debugging  code
			auto DEBUGGIN_retrievedId = Rldx::DxResourceManager::Instance()->GetResourceByString<SHADER_TYPE>("shader01");

			return newInstance;
		}

		virtual ID3D11VertexShader* GetVertexShader()  override { 
			return m_vertexShaderFile.GetShader(); 
		};
		virtual ID3D11PixelShader* GetPixelShader()   override { 
			return m_pixelShaderFile.GetShader(); 
		};
		virtual ID3D11InputLayout* GetInputLayout()   override { 
			return m_vertexShaderFile.GetInputLayout(); 
		};

		ID3D11Buffer* GetPixelShaderConstBuffer() const { return m_pixelShaderConstBuffer.GetBuffer(); };
		ID3D11Buffer* GetVertexShaderConstBuffer() const { return m_vertexShaderConstBuffer.GetBuffer(); };

		VS_CONST_BUFER& GetCSEditableVS() { return vertexShaderConstBuffer; };
		PS_CONST_BUFER& GetCSEditablePS() { return pixelShaderConstBuffer; };
			
		virtual void BindToDC(ID3D11DeviceContext* dc) override {};

		virtual ResourceTypeEnum GetType() const override { return ResourceTypeEnum::ShaderProgram; };

	};

	class DxMeshShaderProgram : public TDxShaderProgram<VS_MeshConstantBuffer, PS_MeshConstantBuffer>
	{
	public:
		/*virtual void Create(ID3D11Device* poDevice, std::wstring vertexShaderPath, std::wstring pixelShaderPath)
		{
			auto newInstance = TDxShaderProgram<VS_MeshConstantBuffer, PS_MeshConstantBuffer>::Create(poDevice, vertexShaderPath, pixelShaderPath);

			auto result = Rldx::DxResourceManager::GetInstance()->GetShaderManager()->AddResource(newInstance, "shader01");

			auto resource = Rldx::DxResourceManager::GetInstance()->GetShaderManager()->GetResourceById(0);

			auto DEBUG_BREAK_POINT = 1;
		}*/


		virtual void BindToDC(ID3D11DeviceContext* deviceContext) override
		{
			// TODO: should any const buffer data be sent from here?
			auto& vsCB = GetCSEditableVS();
			vsCB.mWorld = sm::Matrix::Identity;

			deviceContext->PSSetShader(GetPixelShader(), nullptr, 0);
			deviceContext->VSSetShader(GetVertexShader(), nullptr, 0);

			// TODO: should that be here ? It logically belong with the mesh data, but it ubtain from the vertex shader:(
			deviceContext->IASetInputLayout(GetInputLayout());
		};

	};	

}; // namespace Rldx


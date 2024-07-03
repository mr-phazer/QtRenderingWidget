#pragma once

// stl
#include <d3d11.h>
#include <string>

// MS Direct3d lib
#include "..\..\..\DirectXTK\Inc\BufferHelpers.h"

// author
#include "..\DataTypes\ConstBuffers\CPUConstBuffers.h"
#include "..\DataTypes\ShaderFiles.h"
#include "..\Interfaces\IBindable.h"
#include "..\Logging\Logging.h"
#include "..\Managers\DxDeviceManager.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"
#include "..\Managers\ResourceManager\IDxResource.h"
#include "..\ShaderLoadingHelpers\ShaderLoadingHelpers.h"

#include "Logger\Logger.h"

#include "..\Interfaces\IBindable.h"

namespace rldx {

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
		std::wstring GetTypeString() const override;
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

		std::wstring m_pixelShaderPath = L"";
		std::wstring m_vertexShaderPath = L"";

	public:
		template <typename SHADER_TYPE>
		static auto Create(ID3D11Device* poDevice, std::pair<uint8_t*, size_t> vertexShaderMem, std::pair<uint8_t*, size_t> pixelShaderMem)
		{
			auto newShaderHandle = rldx::DxResourceManager::Instance()->AllocShaderProgram<SHADER_TYPE>("shader01");

			if (vertexShaderMem.first != nullptr && vertexShaderMem.second > 0)
				newShaderHandle.GetPtr()->m_vertexShaderFile = VertexShaderLoader::CreateVertexShaderFromMemory(poDevice, vertexShaderMem.first, vertexShaderMem.second);

			if (pixelShaderMem.first != nullptr && pixelShaderMem.second > 0)
				newShaderHandle.GetPtr()->m_pixelShaderFile = PixelShaderLoader::CreatePixelShaderFromMemory(poDevice, pixelShaderMem.first, pixelShaderMem.second);

			newShaderHandle.GetPtr()->m_pixelShaderConstBuffer.Create(poDevice);
			newShaderHandle.GetPtr()->m_vertexShaderConstBuffer.Create(poDevice);

			return newShaderHandle;
		};


		template <typename SHADER_TYPE>
		static auto Create(ID3D11Device* poDevice, std::wstring vertexShaderPath, std::wstring pixelShaderPath)
		{
			Logger::LogAction(L"Creating Shaders...");

			if (vertexShaderPath.empty() || pixelShaderPath.empty()) {
				return rldx::TResourceHandle<SHADER_TYPE>();
			}

			auto newShaderHandle = rldx::DxResourceManager::Instance()->AllocShaderProgram<SHADER_TYPE>(L"shader01");

			if (!vertexShaderPath.empty())
			{
				logging::LogAction(L"Loading vertex shader: " + vertexShaderPath);
				newShaderHandle.GetPtr()->m_vertexShaderFile = VertexShaderLoader::CreateVertexShaderFromDisk(poDevice, vertexShaderPath);
			}

			if (!pixelShaderPath.empty())
			{
				logging::LogAction(L"Loading pixel shader: " + pixelShaderPath);
				newShaderHandle.GetPtr()->m_pixelShaderFile = PixelShaderLoader::CreatePixelShaderFromDisk(poDevice, pixelShaderPath);
			}

			logging::LogAction(L"D3D Call: Create Shader(s): ");
			newShaderHandle.GetPtr()->m_pixelShaderConstBuffer.Create(poDevice);
			newShaderHandle.GetPtr()->m_vertexShaderConstBuffer.Create(poDevice);

			newShaderHandle.GetPtr()->m_pixelShaderPath = pixelShaderPath;
			newShaderHandle.GetPtr()->m_vertexShaderPath = vertexShaderPath;

			return newShaderHandle;
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

	class DxMeshShaderProgram : public TDxShaderProgram<VS_PerMesh_ConstBuffer, PS_PerMesh_ConstBuffer>
	{
	public:
		/*virtual void Create(ID3D11Device* poDevice, std::wstring vertexShaderPath, std::wstring pixelShaderPath)
		{
			auto newShaderInstance = TDxShaderProgram<VS_MeshConstantBuffer, PS_PerMesh_ConstBuffer>::Create(poDevice, vertexShaderPath, pixelShaderPath);

			auto newShaderHandle = rldx::DxResourceManager::GetInstance()->GetShaderManager()->AddResource(newShaderInstance, "shader01");

			auto resource = rldx::DxResourceManager::GetInstance()->GetShaderManager()->GetResourceById(0);

			auto DEBUG_BREAK_POINT = 1;
		}*/

		virtual void BindToDC(ID3D11DeviceContext* deviceContext) override
		{
			//// TODO: should any const buffer data be sent from here?
			//auto& vsCB = GetCSEditableVS();//vsCB.mWorld = sm::Matrix::Identity;

			deviceContext->PSSetShader(GetPixelShader(), nullptr, 0);
			deviceContext->VSSetShader(GetVertexShader(), nullptr, 0);

			// TODO: should that be here ? It logically belong with the mesh data, but it ubtain from the vertex shader:(
			deviceContext->IASetInputLayout(GetInputLayout());
		};

	};

	class DefaultShaderCreator
	{
		static IntId simpleShader;
		static IntId noTextureShader;

	public:
		static DxMeshShaderProgram* GetSimpleShaderProgram()
		{
			if (simpleShader == INVALID_ID)
			{
				auto newSimpleShaderProgram =
					DxMeshShaderProgram::Create<DxMeshShaderProgram>(
						DxDeviceManager::Device(),
						LR"(VS_Simple.cso)",
						LR"(PS_Simple.cso)");

				simpleShader = newSimpleShaderProgram.GetId();
				return newSimpleShaderProgram.GetPtr();
			}

			return DxResourceManager::Instance()->GetResourceById<DxMeshShaderProgram>(simpleShader);
		}

		static DxMeshShaderProgram* GetNoTextureShaderProgram()
		{
			if (noTextureShader == INVALID_ID)
			{
				auto noTexturesShaderProgram =
					DxMeshShaderProgram::Create<DxMeshShaderProgram>(
						DxDeviceManager::Device(),
						LR"(VS_Simple.cso)",
						LR"(PS_NoTextures.cso)"
					);

				noTextureShader = noTexturesShaderProgram.GetId();
				return noTexturesShaderProgram.GetPtr();
			}

			return DxResourceManager::Instance()->GetResourceById<DxMeshShaderProgram>(noTextureShader);
		}
	};




	// TODO: remove other one, if this is better, else remove this
	//template <typename VS_CONST_BUFER, typename PS_CONST_BUFER>
	//class TDxShaderProgram_NewTest : public IDxShaderProgram
	//{
	//	using Type = TDxShaderProgram<VS_CONST_BUFER, PS_CONST_BUFER>;
	//	PixelShaderFile m_pixelShaderFile;
	//	VertexShaderFile m_vertexShaderFile;

	//	TDxVSShaderConstBuffer<VS_CONST_BUFER> m_vertexShaderConstBuffer;
	//	TDxPSShaderConstBuffer<PS_CONST_BUFER> m_pixelShaderConstBuffer;		

	//public:
	//	template <typename SHADER_TYPE>
	//	static  SHADER_TYPE* Create(ID3D11Device* poDevice, std::pair<uint8_t*, size_t> vertexShaderMem, std::pair<uint8_t*, size_t> pixelShaderMem)
	//	{
	//		auto newShaderHandle = rldx::DxResourceManager::Instance()->AllocShaderProgram<SHADER_TYPE>("shader01");
	//		//auto newShaderInstance = static_cast<TDxShaderProgram*>(newShaderHandle.GetPtr());
	//		auto newShaderInstance = newShaderHandle.GetPtr();

	//		if (vertexShaderMem.first != nullptr && vertexShaderMem.second > 0)
	//			newShaderInstance->m_vertexShaderFile = VertexShaderLoader::CreateVertexShaderFromMemory(poDevice, vertexShaderMem.first, vertexShaderMem.second);

	//		if (pixelShaderMem.first != nullptr && pixelShaderMem.second > 0)
	//			newShaderInstance->m_pixelShaderFile = PixelShaderLoader::CreatePixelShaderFromMemory(poDevice, pixelShaderMem.first, pixelShaderMem.second);

	//		newShaderInstance->m_pixelShaderConstBuffer.Create(poDevice);
	//		newShaderInstance->m_vertexShaderConstBuffer.Create(poDevice);

	//		return newShaderInstance;
	//	};


	//	template <typename SHADER_TYPE>
	//	static SHADER_TYPE* Create(ID3D11Device* poDevice, std::wstring vertexShaderPath, std::wstring pixelShaderPath)
	//	{
	//		auto newShaderHandle = rldx::DxResourceManager::Instance()->AllocShaderProgram<SHADER_TYPE>("shader01");
	//		//auto newShaderInstance = static_cast<TDxShaderProgram*>(newShaderHandle.GetPtr());
	//		auto newShaderInstance = newShaderHandle.GetPtr();

	//		if (vertexShaderPath.empty() || pixelShaderPath.empty()) {
	//			return nullptr;
	//		}

	//		if (!vertexShaderPath.empty())
	//			newShaderInstance->m_vertexShaderFile = VertexShaderLoader::CreateVertexShaderFromDisk(poDevice, vertexShaderPath);

	//		if (!pixelShaderPath.empty())
	//			newShaderInstance->m_pixelShaderFile = PixelShaderLoader::CreatePixelShaderFromDisk(poDevice, pixelShaderPath);

	//		newShaderInstance->m_pixelShaderConstBuffer.Create(poDevice);
	//		newShaderInstance->m_vertexShaderConstBuffer.Create(poDevice);

	//		// TODO: remove debugging  code
	//		auto DEBUGGIN_retrievedId = rldx::DxResourceManager::Instance()->GetResourceByString<SHADER_TYPE>("shader01");

	//		return newShaderInstance;
	//	}

	//	virtual ID3D11VertexShader* GetVertexShader()  override {
	//		return m_vertexShaderFile.GetShader();
	//	};
	//	virtual ID3D11PixelShader* GetPixelShader()   override {
	//		return m_pixelShaderFile.GetShader();
	//	};
	//	virtual ID3D11InputLayout* GetInputLayout()   override {
	//		return m_vertexShaderFile.GetInputLayout();
	//	};

	//	ID3D11Buffer* GetPixelShaderConstBuffer() const { return m_pixelShaderConstBuffer.GetBuffer(); };
	//	ID3D11Buffer* GetVertexShaderConstBuffer() const { return m_vertexShaderConstBuffer.GetBuffer(); };

	//	VS_CONST_BUFER& GetCSEditableVS() { return vertexShaderConstBuffer; };
	//	PS_CONST_BUFER& GetCSEditablePS() { return pixelShaderConstBuffer; };

	//	virtual void BindToDC(ID3D11DeviceContext* dc) override {};

	//	virtual ResourceTypeEnum GetType() const override { return ResourceTypeEnum::ShaderProgram; };

	//};


}; // namespace rldx


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
#include "..\Interfaces\IBindable.h"
#include "..\Managers\DxDeviceManager.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"
#include "..\Managers\ResourceManager\IDxResource.h"
#include "..\ShaderLoadingHelpers\ShaderLoadingHelpers.h"
#include <CommonLibs\Logger\Logger.h>

namespace rldx {

	class IDxShaderProgram : public IDxResource, public IBindable
	{
	public:
		virtual ID3D11VertexShader* GetVertexShader() = 0;
		virtual ID3D11PixelShader* GetPixelShader() = 0;
		virtual ID3D11InputLayout* GetInputLayout() = 0;

		virtual ID3D11Buffer* GetPixelShaderConstBuffer() const = 0;
		virtual ID3D11Buffer* GetVertexShaderConstBuffer() const = 0;
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
		static auto CreateFromMemoery(ID3D11Device* poDevice, DxResourceManager& resourcemanager, std::pair<uint8_t*, size_t> vertexShaderMem, std::pair<uint8_t*, size_t> pixelShaderMem)
		{
			auto newShaderHandle = resourcemanager.CreateResouce<SHADER_TYPE>();

			if (vertexShaderMem.first != nullptr && vertexShaderMem.second > 0)
				newShaderHandle.GetPtr()->m_vertexShaderFile = VertexShaderLoader::CreateVertexShaderFromMemory(poDevice, vertexShaderMem.first, vertexShaderMem.second);

			if (pixelShaderMem.first != nullptr && pixelShaderMem.second > 0)
				newShaderHandle.GetPtr()->m_pixelShaderFile = PixelShaderLoader::CreatePixelShaderFromMemory(poDevice, pixelShaderMem.first, pixelShaderMem.second);

			newShaderHandle->m_pixelShaderConstBuffer.Create(poDevice);
			newShaderHandle->m_vertexShaderConstBuffer.Create(poDevice);

			return newShaderHandle;
		};

		template <typename SHADER_TYPE>
		static SHADER_TYPE* CreateFromDisk(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, const std::wstring& name = L"ShaderProgra")
		{
			logging::LogAction(L"Creating Shaders...");

			if (vertexShaderPath.empty() || pixelShaderPath.empty()) {
				return nullptr;
			}

			auto newShaderPtr = resourceManager.CreateResouce<SHADER_TYPE>();

			if (!vertexShaderPath.empty())
			{
				logging::LogAction(L"Loading vertex shader: " + vertexShaderPath);
				newShaderPtr->m_vertexShaderFile = VertexShaderLoader::CreateVertexShaderFromDisk(poDevice, vertexShaderPath);
			}

			if (!pixelShaderPath.empty())
			{
				logging::LogAction(L"Loading pixel shader: " + pixelShaderPath);
				newShaderPtr->m_pixelShaderFile = PixelShaderLoader::CreatePixelShaderFromDisk(poDevice, pixelShaderPath);
			}

			logging::LogAction(L"D3D Call: Create Shader(s): ");
			newShaderPtr->m_pixelShaderConstBuffer.Create(poDevice);
			newShaderPtr->m_vertexShaderConstBuffer.Create(poDevice);

			newShaderPtr->m_pixelShaderPath = pixelShaderPath;
			newShaderPtr->m_vertexShaderPath = vertexShaderPath;

			return newShaderPtr;
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
	};

	// TODO: CLEAN UP!
	class DxMeshShaderProgram : public TDxShaderProgram<VS_PerMesh_ConstBuffer, PS_PerMesh_ConstBuffer>
	{
	public:
		/*virtual void Create(ID3D11Device* poDevice, std::wstring vertexShaderPath, std::wstring pixelShaderPath)
		{
			auto newShaderInstance = TDxShaderProgram<VS_MeshConstantBuffer, PS_PerMesh_ConstBuffer>::Create(poDevice, vertexShaderPath, pixelShaderPath);

			auto newShaderPtr = rldx::DxResourceManager::GetInstance()->GetShaderManager()->AddResource(newShaderInstance, "shader01");

			auto resource = rldx::DxResourceManager::GetInstance()->GetShaderManager()->GetResourceById(0);

			auto DEBUG_BREAK_POINT = 1;
		}*/

		virtual void BindToDC(ID3D11DeviceContext* deviceContext) override
		{
			//// TODO: should any const buffer data be sent from here?
			//auto& vsCB = GetCSEditableVS();//vsCB.mWorld = sm::Matrix::Identity;

			deviceContext->PSSetShader(GetPixelShader(), nullptr, 0);
			deviceContext->VSSetShader(GetVertexShader(), nullptr, 0);

			// TODO: should that be here ? It logically belong with the mesh data, but it ubtain from the vertex poShader:(
			deviceContext->IASetInputLayout(GetInputLayout());
		};
	};


	// TODO: MAYBE make it so is only loads one instance of the shader, ? AFTER Asset cleanup too?
	class DefaultShaderCreator
	{
		static DxMeshShaderProgram* simpleShader;
		static DxMeshShaderProgram* noTextureShader;

	public:
		static DxMeshShaderProgram* GetSimpleShaderProgram(rldx::DxResourceManager& resourceManager)
		{
			auto newSimpleShaderProgram =
				DxMeshShaderProgram::CreateFromDisk<DxMeshShaderProgram>(
					DxDeviceManager::Device(),
					resourceManager,
					LR"(VS_Simple.cso)",
					LR"(PS_Simple.cso)",
					L"ShaderProgram:Simple"
				);

			return newSimpleShaderProgram;
		}

		static DxMeshShaderProgram* GetNoTextureShaderProgram(rldx::DxResourceManager& resourceManager)
		{
			auto noTexturesShaderProgram =
				DxMeshShaderProgram::CreateFromDisk<DxMeshShaderProgram>(
					DxDeviceManager::Device(),
					resourceManager,
					LR"(VS_Simple.cso)",
					LR"(PS_NoTextures.cso)"
				);

			return noTexturesShaderProgram;
		}
	};


}; // namespace rldx


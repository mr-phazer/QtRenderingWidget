#include "DxResourceManager.h"


// impl of the forward declared resources
#include "..\..\Rendering\DxMesh.h"
#include "..\..\Rendering\DxMaterial.h"
#include "..\..\Rendering\DxTexture.h"
#include "..\..\Rendering\DxShaderProgram.h"


using namespace rldx;

//std::unique_ptr<ResourceManagerBase> ResourceManagerBase::sm_spoInstance;
uint32_t IdCounterBase::sm_nextId;
std::unique_ptr<DxResourceManager> DxResourceManager::sm_spoInstance;


//rldx::ResourceManager::ResourceManager()
//{
//	m_spotextureManager = TResourceManager<DxTextureView>::Create();
//	m_spoMaterialManager = TResourceManager<DxMaterial>::Create();
//	m_spoMeshManager = TResourceManager<DxMesh>::Create();
//}

TResourceHandle<DxTexture> rldx::DxResourceManager::MakeCubemapResource(ID3D11Device* poDevice, const std::wstring& file)
{
	auto resourceHandle = DxResourceManager::Instance()->AllocTexture(file);
	resourceHandle.GetPtr()->LoadCubeMap(poDevice, file);

	return resourceHandle;
}

void rldx::DxResourceManager::SetDefaultShaderProgram(DxMeshShaderProgram* newShaderProgram)
{
	m_defaultShaderProgram = newShaderProgram;
}

DxResourceManager* rldx::DxResourceManager::Instance()
{
	if (!sm_spoInstance) {
		sm_spoInstance = std::unique_ptr<DxResourceManager>(new DxResourceManager);
			}

	return sm_spoInstance.get();
}

TResourceHandle<DxTexture> rldx::DxResourceManager::AllocTexture(const std::wstring& strId)
{
	return AllocEmpty<DxTexture>(strId);
}

TResourceHandle<DxMaterial> rldx::DxResourceManager::AllocMaterial(const std::wstring& strId)
{
	return AllocEmpty<DxMaterial>(strId);
}

TResourceHandle<DxMesh> rldx::DxResourceManager::AllocMesh(const std::wstring& strId)
{
	return AllocEmpty<DxMesh>(strId);
}

TResourceHandle<DxTexture> rldx::DxResourceManager::GetTexture(const std::wstring& strId)
{	
	auto it = m_umapResourcePtrByString.find(strId);

	if (it != m_umapResourcePtrByString.end()) {
		return { it->second->GetId(), static_cast<DxTexture*>(it->second) };
	}

	return { INVALID_ID, nullptr };
}


// TODO: remove
//TResourceManager<DxMesh>* rldx::DxResourceManager::GetMeshes() { return &m_spoMeshManager; }
//
//TResourceManager<DxTexture>* rldx::DxResourceManager::GetTextures() { return &m_spoTextureManager; }
//
//TResourceManager<DxMaterial>* rldx::DxResourceManager::GetMaterialManager() { return &m_spoMaterialManager; }
//
//TResourceManager<IDxShaderProgram>* rldx::DxResourceManager::Shaders() { return &m_spoShaderManager; }

#include "DxResourceManager.h"

#include "..\..\DxObjects\DxMesh.h"
#include "..\..\DxObjects\DxMaterial.h"
#include "..\..\DxObjects\DxShaderProgram.h"



using namespace Rldx;

//std::unique_ptr<ResourceManagerBase> ResourceManagerBase::sm_spoInstance;
uint32_t IdCounter::sm_nextId;
std::unique_ptr<DxResourceManager> DxResourceManager::sm_spoInstance;


//Rldx::ResourceManager::ResourceManager()
//{
//	m_spotextureManager = TResourceManager<DxTextureView>::Create();
//	m_spoMaterialManager = TResourceManager<DxMaterial>::Create();
//	m_spoMeshManager = TResourceManager<DxMesh>::Create();
//}

DxResourceManager* Rldx::DxResourceManager::Instance()
{
	if (!sm_spoInstance) {
		sm_spoInstance = std::unique_ptr<DxResourceManager>(new DxResourceManager);
	}

	return sm_spoInstance.get();
}

TResourceHandle<DxTexture> Rldx::DxResourceManager::AllocTexture(const std::string& strId)
{
	return AddEmpty<DxTexture>(strId);
}


// TODO: remove
//TResourceManager<DxMeshData>* Rldx::DxResourceManager::GetMeshes() { return &m_spoMeshManager; }
//
//TResourceManager<DxTexture>* Rldx::DxResourceManager::GetTextures() { return &m_spoTextureManager; }
//
//TResourceManager<DxMaterial>* Rldx::DxResourceManager::GetMaterialManager() { return &m_spoMaterialManager; }
//
//TResourceManager<IDxShaderProgram>* Rldx::DxResourceManager::Shaders() { return &m_spoShaderManager; }

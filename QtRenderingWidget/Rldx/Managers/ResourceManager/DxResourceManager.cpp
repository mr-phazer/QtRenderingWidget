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

DxResourceManager* rldx::DxResourceManager::Instance()
{
	if (!sm_spoInstance) {
		sm_spoInstance = std::unique_ptr<DxResourceManager>(new DxResourceManager);
	}

	return sm_spoInstance.get();
}

TResourceHandle<DxTexture> rldx::DxResourceManager::AllocTexture(const std::string& strId)
{
	return AddEmpty<DxTexture>(strId);
}

TResourceHandle<DxMaterial> rldx::DxResourceManager::AllocMaterial(const std::string& strId)
{
	return AddEmpty<DxMaterial>(strId);
}

TResourceHandle<DxMesh> rldx::DxResourceManager::AllocMesh(const std::string& strId)
{
	return AddEmpty<DxMesh>(strId);
}


// TODO: remove
//TResourceManager<DxMesh>* rldx::DxResourceManager::GetMeshes() { return &m_spoMeshManager; }
//
//TResourceManager<DxTexture>* rldx::DxResourceManager::GetTextures() { return &m_spoTextureManager; }
//
//TResourceManager<DxMaterial>* rldx::DxResourceManager::GetMaterialManager() { return &m_spoMaterialManager; }
//
//TResourceManager<IDxShaderProgram>* rldx::DxResourceManager::Shaders() { return &m_spoShaderManager; }

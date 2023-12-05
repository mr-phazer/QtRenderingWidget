#include "DxResourceManager.h"

#include "..\DxObjects\DxMesh.h"
#include "..\DxObjects\DxMaterial.h"
#include "..\DxObjects\DxShaderProgram.h"



using namespace Rldx;

//std::unique_ptr<ResourceManagerBase> ResourceManagerBase::sm_spoInstance;
uint32_t ResourceManagerBase::sm_nextId;
std::unique_ptr<DxResourceManager> DxResourceManager::sm_spoInstance;


//inline Rldx::ResourceManager::ResourceManager()
//{
//	m_spotextureManager = TResourceManager<DxTextureView>::Create();
//	m_spoMaterialManager = TResourceManager<DxMaterial>::Create();
//	m_spoMeshManager = TResourceManager<DxMesh>::Create();
//}

inline DxResourceManager* Rldx::DxResourceManager::GetInstance()
{
	if (!sm_spoInstance) {
		sm_spoInstance = std::unique_ptr<DxResourceManager>(new DxResourceManager);
	}

	return sm_spoInstance.get();
}

inline TResourceManager<DxMeshData>* Rldx::DxResourceManager::GetMeshes() { return &m_spoMeshManager; }

inline TResourceManager<DxTexture>* Rldx::DxResourceManager::GetTextures() { return &m_spoTextureManager; }

inline TResourceManager<DxMaterial>* Rldx::DxResourceManager::GetMaterialManager() { return &m_spoMaterialManager; }

inline TResourceManager<IDxShaderProgram>* Rldx::DxResourceManager::GetShaderManager() { return &m_spoShaderManager; }

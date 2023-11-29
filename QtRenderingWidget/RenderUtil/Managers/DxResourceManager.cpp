#include "DxResourceManager.h"

using namespace Rldx;

//std::unique_ptr<ResourceManagerBase> ResourceManagerBase::sm_spoInstance;
ResId ResourceManagerBase::sm_nextId;
std::unique_ptr<ResourceManager> ResourceManager::sm_spoInstance;


//inline Rldx::ResourceManager::ResourceManager()
//{
//	m_spotextureManager = TResourceManager<DxTextureView>::Create();
//	m_spoMaterialManager = TResourceManager<DxMaterial>::Create();
//	m_spoMeshManager = TResourceManager<DxMesh>::Create();
//}

inline ResourceManager& Rldx::ResourceManager::Get()
{
	if (!sm_spoInstance) {
		sm_spoInstance = std::unique_ptr<ResourceManager>(new ResourceManager);
	}

	return *sm_spoInstance;
}

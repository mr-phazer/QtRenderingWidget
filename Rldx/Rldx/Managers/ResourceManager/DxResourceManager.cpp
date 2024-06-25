#include "DxResourceManager.h"

#include "..\..\Animation\DataTypes\SkeletonAnimation.h"
#include "..\..\Rendering\DxMaterial.h"
#include "..\..\Rendering\DxMesh.h"
#include "..\..\Rendering\DxShaderProgram.h"
#include "..\..\Rendering\DxTexture.h"


namespace rldx
{
	uint32_t IdCounterBase::sm_nextId;
	std::unique_ptr<DxResourceManager> DxResourceManager::sm_spoInstance;


	TResourceHandle<DxTexture> DxResourceManager::MakeCubemapResource(ID3D11Device* poDevice, const std::wstring& file)
	{
		auto resourceHandle = DxResourceManager::Instance()->AllocTexture(file);
		resourceHandle.GetPtr()->LoadCubeMap(poDevice, file);

		return resourceHandle;
	}

	void DxResourceManager::SetDefaultShaderProgram(DxMeshShaderProgram* newShaderProgram)
	{
		m_defaultShaderProgram = newShaderProgram;
	}

	DxResourceManager* DxResourceManager::Instance()
	{
		if (!sm_spoInstance) {
			sm_spoInstance = std::unique_ptr<DxResourceManager>(new DxResourceManager);
		}

		return sm_spoInstance.get();
	}

	TResourceHandle<DxTexture> DxResourceManager::AllocTexture(const std::wstring& strId)
	{
		return AllocEmpty<DxTexture>(strId);
	}

	TResourceHandle<DxMaterial> DxResourceManager::AllocMaterial(const std::wstring& strId)
	{
		return AllocEmpty<DxMaterial>(strId);
	}

	TResourceHandle<DxMesh> DxResourceManager::AllocMesh(const std::wstring& strId)
	{
		return AllocEmpty<DxMesh>(strId);
	}

	TResourceHandle<skel_anim::SkeletonAnimation> DxResourceManager::AllocAnim(const std::wstring& strId)
	{
		return AllocEmpty<skel_anim::SkeletonAnimation>(strId);
	}

	TResourceHandle<DxTexture> DxResourceManager::GetTexture(const std::wstring& strId)
	{
		auto it = m_umapResourcePtrByString.find(strId);

		if (it != m_umapResourcePtrByString.end()) {
			return { it->second->GetId(), static_cast<DxTexture*>(it->second) };
		}

		return { INVALID_ID, nullptr };
	}
}
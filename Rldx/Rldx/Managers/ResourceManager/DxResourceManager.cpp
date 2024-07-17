#include "DxResourceManager.h"

#include "..\..\Animation\DataTypes\SkeletonAnimation.h"
#include "..\..\Rendering\DxMaterial.h"
#include "..\..\Rendering\DxMesh.h"
#include "..\..\Rendering\DxShaderProgram.h"
#include "..\..\Rendering\DxTexture.h"


namespace rldx
{
	IntId IdCounterBase::sm_nextId;
	std::unique_ptr<DxResourceManager> DxResourceManager::sm_spoInstance;

	void DxResourceManager::FreeAll()
	{
		auto itr = Instance()->m_umapResourceSptrDataById.begin();
		while (itr != Instance()->m_umapResourceSptrDataById.end())
		{
			if (itr->second) {
				itr = Instance()->m_umapResourceSptrDataById.erase(itr);
				break;
			}
			else
			{
				itr++;
			}
		}

		auto ItStringMap = Instance()->m_umapRawResourcePtrByString.begin();
		while (ItStringMap != Instance()->m_umapRawResourcePtrByString.end())
		{
			if (ItStringMap->second)
			{
				ItStringMap = Instance()->m_umapRawResourcePtrByString.erase(ItStringMap);
				break;
			}
			else
			{
				ItStringMap++;
			}
		}
	}

	TResourceHandle<DxTexture> DxResourceManager::MakeCubemapResource(ID3D11Device* poDevice, const std::wstring& file)
	{
		auto resourceHandle = DxResourceManager::Instance()->AllocTexture(file, AllocTypeEnum::AttempReuseIdForNew);
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

	TResourceHandle<DxTexture> DxResourceManager::AllocTexture(const std::wstring& strId, AllocTypeEnum allocType)
	{
		return AllocEmpty<DxTexture>(strId, allocType);
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
		auto it = m_umapRawResourcePtrByString.find(strId);

		if (it != m_umapRawResourcePtrByString.end()) {
			return { it->second->GetId(), static_cast<DxTexture*>(it->second) };
		}

		return { INVALID_ID, nullptr };
	}



	void DxResourceManager::FreeMemoryByPtrFromIdMap(IDxResource* resource)
	{
		auto itr = Instance()->m_umapResourceSptrDataById.begin();
		while (itr != Instance()->m_umapResourceSptrDataById.end())
		{
			if (itr->second.get() == resource) {
				itr = Instance()->m_umapResourceSptrDataById.erase(itr);
				break;
			}
			else
			{
				itr++;
			}
		}
	}

	void DxResourceManager::FreeMemorByPtrFromStringMap(IDxResource* resource)
	{
		auto itr = Instance()->m_umapRawResourcePtrByString.begin();
		while (itr != Instance()->m_umapRawResourcePtrByString.end())
		{
			if (itr->second == resource) {
				itr = Instance()->m_umapRawResourcePtrByString.erase(itr);
				break;
			}
			else
			{
				itr++;
			}
		}
	}

}
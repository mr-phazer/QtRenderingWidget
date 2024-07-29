#include "DxResourceManager.h"

#include "..\..\Animation\DataTypes\SkeletonAnimation.h"
#include "..\..\Rendering\DxMaterial.h"
#include "..\..\Rendering\DxMesh.h"
#include "..\..\Rendering\DxShaderProgram.h"
#include "..\..\Rendering\DxTexture.h"


namespace rldx
{
	/*IntId IdCounterBase::sm_nextId = 0;
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

	TResourceHandle<DxTexture> DxResourceManager::GetTexture_OLD(const std::wstring& strId)
	{
		auto it = m_umapRawResourcePtrByString.find(strId);

		if (it != m_umapRawResourcePtrByString.end()) {
			return { it->second->GetId(), static_cast<DxTexture*>(it->second) };
		}

		return { INVALID_ID, nullptr };
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
	}*/

	AssetFetchCallbackWrapper DxResourceManager::sm_assetCallBack;

	void DxResourceManager::DestroyAllResources()
	{
		m_umapResources.clear();

		// TODO: REMOVE if above works!
		/*auto itr = m_umapResources.begin();
		while (itr != m_umapResources.end())
		{
			if (itr->second) {
				itr = m_umapResources.erase(itr);
			}
			else
			{
				itr++;
			}
		}*/
	}

	void DxResourceManager::GetResourcesFromCallBack(std::vector<std::wstring>& qstrMissingFiles, std::vector<std::vector<unsigned char>>& destBinaries)
	{
		if (!sm_assetCallBack) {
			throw std::exception("No asset callback function set");
		}

		sm_assetCallBack(&qstrMissingFiles, &destBinaries);
	}

	utils::ByteStream DxResourceManager::GetFileFromCallBack(const std::wstring& fileName)
	{
		std::vector<std::wstring> qstrMissingFiles;
		std::vector<std::vector<unsigned char>> destBinaries;

		qstrMissingFiles.push_back(fileName);

		GetResourcesFromCallBack(qstrMissingFiles, destBinaries); // fetch from callback

		if (destBinaries.size() != 1)
		{
			throw std::exception(std::string(FULL_FUNC_INFO("ERROR: File count mismatch (should be 1)")).c_str());
		}

		auto binary = destBinaries.at(0);
		if (binary.empty())
		{
			// TODO: CLEAN UP
			//throw std::exception(string(FULL_FUNC_INFO("ERROR: File: '" + libtools::wstring_to_string(fileName) + "', is empty or couldn't be found")).c_str());
			return utils::ByteStream();
		}

		// TODO: CLEAN UP
		//			Logger::LogActionSuccess(L"Found packed file (through callback): " + fileName);

		return utils::ByteStream(binary.data(), binary.size(), fileName);
	}

	utils::ByteStream DxResourceManager::GetFile(const std::wstring& filePath)
	{
		auto bytes = GetFileFromCallBack(filePath); // fetch from callback
		if (!bytes.IsValid()) // no file found, try to load from disk
		{
			return utils::ByteStream(filePath, false);
		}

		return bytes;
	}

	void DxResourceManager::RemoveResourceFromMap(IDxResource* resource)
	{
		auto itr = m_umapResources.begin();
		while (itr != m_umapResources.end())
		{
			if (itr->second.get() == resource) {
				itr = m_umapResources.erase(itr);
				break;
			}
			else
			{
				itr++;
			}
		}
	}

	std::wstring DxResourceManager::sm_rooPathAssetPath;
}

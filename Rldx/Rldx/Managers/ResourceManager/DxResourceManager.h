#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION  0x0800
#endif // !DIRECTINPUT_VERSION

#include <d3d11.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

#include "IDxResource.h"

// author heade
#include <CommonLibs\Logger\Logger.h>
#include <CommonLibs\Utils\MapUtils.h>
#include <CommonLibs\Utils\ByteStream.h>
#include <CommonLibs\Utils\IOUtils.h>

namespace rldx {

	typedef void (*AssetFetchCallbackWrapper) (
		std::vector<std::wstring>* filesToFetch,
		std::vector<std::vector<unsigned char>>* outBinFiles
	);

	class DxResourceManager
	{
		std::map<uint32_t, std::unique_ptr<rldx::IDxResource>> m_umapResources;

		// TODO: Does this belong here? Maybe move to a separate class? Like a "FileLoader" class? Alongside with "GetFile"?
		static std::wstring sm_rooPathAssetPath;// = LR"(c:/temp/)";

		std::wstring m_gameIdString = L"";

	public:
		void SetGameIdSting(const std::wstring& id) { m_gameIdString = id; }
		std::wstring GetGameIdString() const { return m_gameIdString; }

		template<typename Derived_t>
		Derived_t* CreateResouce();

		template <typename T>
		void DestroyResource(T* poResource);

		void DestroyAllResources();

		static void GetResourcesFromCallBack(std::vector<std::wstring>* qstrMissingFiles, std::vector<std::vector<unsigned char>>* destBinaries);

		// TODO: Move to a separate class? like a new class "FileLoader" / similar?
		static utils::ByteStream GetFileFromCallBack(const std::wstring& fileName);
		static utils::ByteStream GetFile(const std::wstring& filePath);

		//static void SetAnimPathsBySkeletonCallBack(AnimPathsBySkeletonCallBack animPathsBySkeletonCallBackFunc) { sm_animPathsBySkeletonCallBack = animPathsBySkeletonCallBackFunc; }
		static void SetAssetFetchCallback(AssetFetchCallbackWrapper assetCallBackFunc) { sm_assetCallBack = assetCallBackFunc; }
		static void CallAssetFetchCallBack(std::vector<std::wstring>* qstrMissingFiles, std::vector<std::vector<unsigned char>>* destBinaries) { GetResourcesFromCallBack(qstrMissingFiles, destBinaries); };

		static void SetGameAssetFolder(const std::wstring& path) { sm_rooPathAssetPath = path; }
		static const std::wstring& GetGameAssetFolder() { return sm_rooPathAssetPath; }

		static AssetFetchCallbackWrapper sm_assetCallBack;
		//static std::function<void(QString*, QList<QString>*)> sm_animPathsBySkeletonCallBack;
	private:
		void RemoveResourceFromMap(IDxResource* resource);

		static std::unique_ptr<DxResourceManager> sm_spoInstance;
	};

	template<typename Derived_t>
	inline Derived_t* DxResourceManager::CreateResouce()
	{
		static_assert(std::is_base_of<IDxResource, Derived_t>::value, "Error: 'Derived_t' is not derived from `IDxResource`");

		auto upoNewResource = std::make_unique<Derived_t>();
		auto poRawResource = upoNewResource.get();

		// TODO: maybe change this, so the id used it not from the resourece, in case it is changed/not set
		auto resourceid = poRawResource->GetId();

		if (m_umapResources.find(resourceid) != m_umapResources.end()) {
			throw std::exception("FATAL Error: Resource with id already exists");
		}

		// adding resource to map, the object is now managing its lifetime
		m_umapResources[resourceid] = std::move(upoNewResource);

		return poRawResource;
	}

	template<typename T>
	inline void DxResourceManager::DestroyResource(T* poResource)
	{
		static_assert(std::is_base_of_v<rldx::IDxResource, T>, "T must be derived from Base");

		if (!poResource)
			return;

		auto poBasePtr = static_cast<rldx::IDxResource*> (poResource);

		RemoveResourceFromMap(poBasePtr);
	}
}; // namespace rldx

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

// author heade
#include <Logger/Logger.h>
#include "..\..\Helpers\StringKeyMap.h"
#include "..\..\QtRenderingWidget\ExternFunctions\Callbacks.h"
#include "IDxResource.h"
#include "Utils\ByteStream.h"
#include "Utils\IOUtils.h"

namespace skel_anim {
	struct SkeletonAnimation;
}

namespace rldx {

	class DxMesh;
	class DxTexture;
	class DxMaterial;
	class IDxShaderProgram;
	class DxMeshShaderProgram;

	template <typename RESOURCE_TYPE>
	class TResourceHandle
	{
	public:
		TResourceHandle() = default;
		TResourceHandle(IntId id, RESOURCE_TYPE* poResource, std::wstring stringID = L"") : m_id(id), m_poResource(poResource), m_stringId(stringID) {};

		IntId GetId() const { return m_id; };
		RESOURCE_TYPE* GetPtr() const { return m_poResource; };
		std::wstring* GetStringKey() const { return m_stringId; };

	private:
		std::wstring m_stringId = L"";  // pointer to string key
		IntId m_id = INVALID_ID;
		RESOURCE_TYPE* m_poResource = nullptr;
	};

	// non-template parent, SHOULD? make sure that the static id is unique per instantiation?
	class IdCounterBase
	{
	protected:
		IntId GetNextId() const { return sm_nextId++; }

	private:
		static IntId sm_nextId;
	};

	class DxResourceManager : public IdCounterBase
	{
	public:
		enum class AllocTypeEnum { AttempReuseIdForNew, MakeNew, AttempUseExisting };

		std::wstring m_gameIdString = L"";

	public:

		static void FreeAll();

		template <typename T>
		static void FreeMemoryFromPtr(T* poResource)
		{
			static_assert(std::is_base_of_v<rldx::IDxResource, T>, "T must be derived from Base");

			if (!poResource)
				return;

			auto poBasePtr = static_cast<rldx::IDxResource*> (poResource);
			FreeMemorByPtrFromStringMap(poBasePtr); // string map first, as it contains non-owning porinters, would be invalidated.
			FreeMemoryByPtrFromIdMap(poBasePtr);
		}






		/// <summary>
		/// TODO: move this to ByteStream?
		/// </summary>		
		static void SetGameAssetFolder(const std::wstring& path) { Instance()->m_rooPathAssetPath = path; }

		/// <summary>
		/// TODO: move this to ByteStream?
		/// </summary>		
		static std::wstring GetGameAssetFolder() { return Instance()->m_rooPathAssetPath; }

		/// <summary>
		/// TODO: move this to ByteStream?
		/// </summary>		
		static std::wstring GetDefaultAssetFolder() { return utils::GetExePath(); }

		void SetGameIdSting(const std::wstring& id) { m_gameIdString = id; }
		std::wstring GetGameIdString() const { return m_gameIdString; }

		static void SetAnimPathsBySkeletonCallBack(AnimPathsBySkeletonCallBack animPathsBySkeletonCallBackFunc) { Instance()->m_animPathsBySkeletonCallBack = animPathsBySkeletonCallBackFunc; }

		static void SetAssetFetchCallback(AssetFetchCallBack assetCallBackFunc) { Instance()->m_assetCallBack = assetCallBackFunc; }
		static void CallAssetFetchCallBack(QList<QString>& qstrMissingFiles, QList<QByteArray>& destBinaries) { Instance()->GetResourcesFromCallBack(qstrMissingFiles, destBinaries); };
		static utils::ByteStream GetFile(const std::wstring& filePath)
		{
			if (utils::IsDiskFile(filePath))
			{
				return utils::ByteStream(filePath);
			}
			else {

				return GetFileFromCallBack(filePath);
			}
		}

		// TODO: is this serving any purpose?
		void AddMissingFile(const std::wstring& file) { m_qstrMissingFiles.push_back(QString::fromStdWString(file)); }

		// TODO: make work?
		TResourceHandle<DxTexture> MakeCubemapResource(ID3D11Device* poDevice, const std::wstring& file);

		void SetDefaultShaderProgram(DxMeshShaderProgram* newShaderProgram);
		DxMeshShaderProgram* GetDefaultShaderProgram() const { return m_defaultShaderProgram; }

		~DxResourceManager()
		{
			// TODO: REMOVE
			auto DEBUG_1 = 1;
		}

		static DxResourceManager* Instance();

		template<typename Derived_t>
		TResourceHandle<Derived_t> Create(const std::wstring& stringId = L"");

		template<typename Derived_t>
		TResourceHandle<Derived_t> AllocEmpty(const std::wstring& stringId, AllocTypeEnum allotype = AllocTypeEnum::AttempUseExisting);

		template<typename Derived_t>
		TResourceHandle<Derived_t> Alloc(const std::wstring& stringId = L"");

		template <typename Derived_t>
		Derived_t* GetResourceByString(const std::wstring& strId) const;

		template <typename Derived_t>
		Derived_t* GetResourceById(IntId id) const;

		template <typename SHADER_TYPE>
		TResourceHandle<SHADER_TYPE> AllocShaderProgram(const std::wstring& strId)
		{
			return AllocEmpty<SHADER_TYPE>(strId);
		};

		TResourceHandle<DxTexture> AllocTexture(const std::wstring& strId, AllocTypeEnum allocType = AllocTypeEnum::AttempUseExisting);
		TResourceHandle<DxMaterial> AllocMaterial(const std::wstring& strId = L"");;
		TResourceHandle<DxMesh> AllocMesh(const std::wstring& strId = L"");
		TResourceHandle<skel_anim::SkeletonAnimation> AllocAnim(const std::wstring& strId = L"");

		/// <summary>
		/// Get texture resource handle is it exists, or {INVALID_HANDLE, nullptr} if it does not
		/// </summary>		
		TResourceHandle<DxTexture> GetTexture(const std::wstring& strId = L"");


	private:
		static void FreeMemoryByPtrFromIdMap(IDxResource* resource);
		static void FreeMemorByPtrFromStringMap(IDxResource* resource);

		static utils::ByteStream GetFileFromCallBack(const std::wstring& fileName)
		{
			QList<QString> qstrMissingFiles = { QString::fromStdWString(fileName) };
			QList<QByteArray> destBinaries;

			Instance()->GetResourcesFromCallBack(qstrMissingFiles, destBinaries); // fetch from callback

			if (destBinaries.size() != 1)
			{
				throw std::exception(std::string(FULL_FUNC_INFO("ERROR: File count mismatch (should be 1)")).c_str());
			}

			if (destBinaries[0].isEmpty())
			{
				// TODO: CLEAN UP
				//throw std::exception(string(FULL_FUNC_INFO("ERROR: File: '" + libtools::wstring_to_string(fileName) + "', is empty or couldn't be found")).c_str());
				return utils::ByteStream();
			}

			// TODO: CLEAN UP
			//			Logger::LogActionSuccess(L"Found packed file (through callback): " + fileName);

			return utils::ByteStream(destBinaries[0].data(), destBinaries[0].size(), fileName);
		}

		void GetResourcesFromCallBack(QList<QString>& qstrMissingFiles, QList<QByteArray>& destBinaries)
		{
			if (!m_assetCallBack) {
				throw std::exception("No asset callback function set");
			}

			m_assetCallBack(&qstrMissingFiles, &destBinaries);
		}

	private:
		DxMeshShaderProgram* m_defaultShaderProgram = nullptr;

		std::map<IntId, std::unique_ptr<IDxResource>> m_umapResourceSptrDataById;
		WStringkeyMap<IDxResource*> m_umapRawResourcePtrByString;

		static std::unique_ptr<DxResourceManager> sm_spoInstance;
		std::wstring m_rooPathAssetPath = LR"(c:/temp/)";
		std::function<void(QList<QString>*, QList<QByteArray>*)> m_assetCallBack;
		std::function<void(QString*, QList<QString>*)> m_animPathsBySkeletonCallBack;
		QVector<QString> m_qstrMissingFiles;
	};

	template<typename Derived_t>
	inline TResourceHandle<Derived_t> DxResourceManager::Create(const std::wstring& stringId)
	{
		auto pResource = GetResourceByString<Derived_t>(stringId);
		if (pResource) {
			return { pResource->GetId(), pResource };
		}

		return TResourceHandle<Derived_t>();
	}

	/// <summary>
	/// Create a new resource of the specified type and stores it in the resource manager.
	/// </summary>
	/// <typeparam m_nodeName="Derived_t">The resource, hase to be Derived from IDxResource</typeparam>
	/// <param m_nodeName="stringId">String Id - Optional</param>
	/// <returns>returns a handle that contains both the pointer to the new resourec, and its global ID</returns>
	template<typename Derived_t>
	inline TResourceHandle<Derived_t> DxResourceManager::AllocEmpty(const std::wstring& stringId, AllocTypeEnum allocType)
	{
		static_assert(std::is_base_of<IDxResource, Derived_t>::value, "Error: 'Derived_t' is not derived from `IDxResource`");

		// TODO: DEBUGGING CODE: makes every alloc "always make New" as before. Make the out-commented code work if possible
		//if ((allocType == AllocTypeEnum::AttempUseExisting || allocType == AllocTypeEnum::AttempReuseIdForNew) && !stringId.empty())
		//{
		//	auto itExistingResource = m_umapRawResourcePtrByString.find(stringId);
		//	if (itExistingResource != m_umapRawResourcePtrByString.end()) // is resource alreadyd loaded
		//	{
		//		auto tempResourceId = itExistingResource->second->GetId();

		//		if (allocType == AllocTypeEnum::AttempUseExisting) // return existing resource			
		//		{
		//			return TResourceHandle<Derived_t>(tempResourceId, static_cast<Derived_t*>(itExistingResource->second), stringId);
		//		}

		//		if (allocType == AllocTypeEnum::AttempReuseIdForNew)
		//		{
		//			auto upoDerived = std::make_unique<Derived_t>(); // allocate new resource mem				

		//			// update the maps with the new resource reusing the keys
		//			auto poRawResource = upoDerived.get();
		//			m_umapRawResourcePtrByString[stringId] = poRawResource;
		//			m_umapResourceSptrDataById[tempResourceId] = std::move(upoDerived);

		//			return TResourceHandle<Derived_t>(tempResourceId, static_cast<Derived_t*>(poRawResource), stringId);
		//		}
		//	}
		//}

		// TODO: move "GetNextId()", and its "static IntId sm_NextId" to IDxResource, makes much more sense
		// create new resource
		auto resourceId = GetNextId();
		auto upoNewResource = std::make_unique<Derived_t>();
		upoNewResource->SetId(resourceId); // manually set resource interal id to be = the global id just generated

		auto poRawResource = upoNewResource.get();
		m_umapResourceSptrDataById[resourceId] = std::move(upoNewResource);

		if (!stringId.empty()) { // associate raw ptr with string id

			auto it = m_umapRawResourcePtrByString.insert(std::make_pair(stringId, poRawResource));
		}

		return TResourceHandle<Derived_t>(resourceId, static_cast<Derived_t*>(poRawResource), stringId);
	}

	template<typename Derived_t>
	inline Derived_t* DxResourceManager::GetResourceByString(const::std::wstring& strId) const
	{
		auto it = m_umapRawResourcePtrByString.find(strId);
		if (it != m_umapRawResourcePtrByString.end()) {

			return static_cast<Derived_t*>(it->second);
		}

		return nullptr;
	}

	template<typename Derived_t>
	inline Derived_t* DxResourceManager::GetResourceById(IntId id) const
	{
		auto it = m_umapResourceSptrDataById.find(id);
		if (it != m_umapResourceSptrDataById.end()) {

			return static_cast<Derived_t*>(it->second.get());
		}

		return nullptr;
	}


}; // namespace rldx
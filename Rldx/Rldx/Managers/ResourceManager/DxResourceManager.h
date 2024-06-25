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
#include "..\..\Helpers\StringKeyMap.h"
#include "..\..\QtRenderingWidget\ExternFunctions\Callbacks.h"
#include "IDxResource.h"
#include <Logger/Logger.h>
#include "Utils\ByteStream.h"
#include "Utils\IOUtils.h"

namespace skel_anim {
	struct SkeletonAnimation;
}

namespace rldx {
	using namespace logging;

	// forward declarations

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
		TResourceHandle(uint32_t id, RESOURCE_TYPE* ptr, std::wstring stringID = L"") : m_id(id), m_ptr(ptr), m_stringID(stringID) {};

		uint32_t GetId() const { return m_id; };
		RESOURCE_TYPE* GetPtr() const { return m_ptr; };
		std::wstring* GetStringKey() const { return m_stringID; };

	private:
		std::wstring m_stringID = L"";  // pointer to string key
		int32_t m_id = INVALID_ID;
		RESOURCE_TYPE* m_ptr = nullptr;
	};

	// non-template parent, SHOULD? make sure that the static id is unique per instantiation?
	class IdCounterBase
	{
	protected:
		uint32_t GetNextId() const { return sm_nextId++; }

	private:
		static uint32_t sm_nextId;
	};

	class DxResourceManager : public IdCounterBase
	{
		std::wstring m_gameIdString = L"";

	public:
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
		static std::wstring GetDefaultAssetFolder() { return GetExePath(); }

		void SetGameIdSting(const std::wstring& id) { m_gameIdString = id; }
		std::wstring GetGameIdSting() { return m_gameIdString; }

		static void SetAnimPathsBySkeletonCallBack(AnimPathsBySkeletonCallBack animPathsBySkeletonCallBackFunc) { Instance()->m_animPathsBySkeletonCallBack = animPathsBySkeletonCallBackFunc; }

		static void SetAssetFetchCallback(AssetFetchCallBack assetCallBackFunc) { Instance()->m_assetCallBack = assetCallBackFunc; }
		static void CallAssetFetchCallBack(QList<QString>& qstrMissingFiles, QList<QByteArray>& destBinaries) { Instance()->GetResourcesFromCallBack(qstrMissingFiles, destBinaries); };
		static ByteStream GetFile(const std::wstring& filePath)
		{
			if (libtools::IsDiskFile(filePath))
			{
				return ByteStream(filePath);
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
		TResourceHandle<Derived_t> AllocEmpty(const std::wstring& stringId = L"");

		template<typename Derived_t>
		TResourceHandle<Derived_t> Alloc(const std::wstring& stringId = L"");

		template <typename Derived_t>
		Derived_t* GetResourceByString(const std::wstring& strId) const;

		template <typename Derived_t>
		Derived_t* GetResourceById(uint32_t id) const;

		template <typename SHADER_TYPE>
		TResourceHandle<SHADER_TYPE> AllocShaderProgram(const std::wstring& strId = "")
		{
			return AllocEmpty<SHADER_TYPE>(strId);
		};

		TResourceHandle<DxTexture> AllocTexture(const std::wstring& strId = L"");
		TResourceHandle<DxMaterial> AllocMaterial(const std::wstring& strId = L"");;
		TResourceHandle<DxMesh> AllocMesh(const std::wstring& strId = L"");
		TResourceHandle<skel_anim::SkeletonAnimation> AllocAnim(const std::wstring& strId = L"");

		/// <summary>
		/// Get texture resource handle is it exists, or {INVALID_HANDLE, nullptr} if it does not
		/// </summary>		
		TResourceHandle<DxTexture> GetTexture(const std::wstring& strId = L"");


	private:
		static ByteStream GetFileFromCallBack(const std::wstring& fileName)
		{
			QList<QString> qstrMissingFiles = { QString::fromStdWString(fileName) };
			QList<QByteArray> destBinaries;

			Instance()->GetResourcesFromCallBack(qstrMissingFiles, destBinaries); // fetch from callback

			if (destBinaries.size() != 1)
			{
				throw std::exception(string(FULL_FUNC_INFO("ERROR: File count mismatch (should be 1)")).c_str());
			}

			if (destBinaries[0].isEmpty())
			{
				//throw std::exception(string(FULL_FUNC_INFO("ERROR: File: '" + libtools::wstring_to_string(fileName) + "', is empty or couldn't be found")).c_str());
				return ByteStream();
			}

			return ByteStream(destBinaries[0].data(), destBinaries[0].size(), fileName);
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

		std::map<uint32_t, std::shared_ptr<IDxResource>> m_umapResourceSptrDataById;
		WStringkeyMap<IDxResource*> m_umapResourcePtrByString;

		/*template <typename VALUE>
		using StringkeyMap
			= typename
		std::unordered_map<
			std::wstring,
			IDxResource*,
			CaseInsensitiveUnorderedMapComparer::Hash,
			CaseInsensitiveUnorderedMapComparer::Compare> m_umapResourcePtrByString;

		using MapInterator =
			std::unordered_map<
			std::wstring,
			IDxResource*,
			CaseInsensitiveUnorderedMapComparer::Hash,
			CaseInsensitiveUnorderedMapComparer::Compare>::iterator;*/

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
	/// <typeparam name="Derived_t">The resource, hase to be Derived from IDxResource</typeparam>
	/// <param name="stringId">String Id - Optional</param>
	/// <returns>returns a handle that contains both the pointer to the new resourec, and its global ID</returns>
	template<typename Derived_t>
	inline TResourceHandle<Derived_t> DxResourceManager::AllocEmpty(const std::wstring& stringId)
	{
		static_assert(std::is_base_of<IDxResource, Derived_t>::value, "Errror: 'Derived_t' is derived from `IDxResource`");

		auto resourceId = GetNextId();

		auto spoDerived = std::make_shared<Derived_t>();
		spoDerived->SetId(resourceId); // manually set resource interal id to be = the global id just generated

		m_umapResourceSptrDataById[resourceId] = spoDerived;;

		// optional stringifd -> raw pointer association
		if (!stringId.empty()) {
			auto it = m_umapResourcePtrByString.insert(std::make_pair(stringId, spoDerived.get()));
		}

		return TResourceHandle<Derived_t>(resourceId, spoDerived.get(), stringId);
	}

	template<typename Derived_t>
	inline Derived_t* DxResourceManager::GetResourceByString(const::std::wstring& strId) const
	{
		auto it = m_umapResourcePtrByString.find(strId);
		if (it != m_umapResourcePtrByString.end()) {

			return static_cast<Derived_t*>(it->second);
		}

		return nullptr;
	}

	template<typename Derived_t>
	inline Derived_t* DxResourceManager::GetResourceById(uint32_t id) const
	{
		auto it = m_umapResourceSptrDataById.find(id);
		if (it != m_umapResourceSptrDataById.end()) {

			return static_cast<Derived_t*>(it->second.get());
		}

		return nullptr;
	}
}; // namespace rldx

// TODO: remove once all usuable code is moved to DxResourceManager
//template <typename RESOURCE_TYPE>
//class TResourceManager : public ResourceManagerBase
//{
//public:
//
//	/*RESOURCE_TYPE* GetResourceById(uint32_t id) const
//	{
//		auto it = m_umapResourceSptrDataById.find(id);
//		if (it != m_umapResourceSptrDataById.end())
//		{
//			return it->second.get();
//		}
//
//		return nullptr;
//	};
//
//	RESOURCE_TYPE* GetResourceByString(const std::wstring& strId) const
//	{
//		auto it = m_umapResourcePtrByString.find(strId);
//		if (it != m_umapResourcePtrByString.end()) {
//			return it->second;
//		}
//
//		return nullptr;
//	};*/
//
//	//template <typename Derived_t>
//	//TResourceHandle<Derived_t> AllocEmpty(const std::wstring& stringId)
//	//{
//	//	auto resourceId = GetNextId();
//	//	auto pDerived = new Derived_t;
//	//	m_umapResourceSptrDataById[resourceId] = std::shared_ptr<RESOURCE_TYPE>(pDerived);
//	//	auto pAllocatedResource = m_umapResourceSptrDataById[resourceId].get();
//
//	//	// if string supplied, associate string with raw pointer to resource
//	//	if (!stringId.empty()) {
//	//		m_umapResourcePtrByString[stringId] = pAllocatedResource; //m_umapResourceSptrDataById[resourceId].get();			
//	//	}			
//
//	//	return { resourceId, pDerived};
//	//}
//
//
//	//template <typename Derived_t>
//	//ResourceHandle<Derived_t> Add(Derived_t* resource, const std::wstring& stringId = "")
//	//{
//	//	auto resourceId = GetNextId();
//	//	m_umapResourceSptrDataById[resourceId] = std::shared_ptr<RESOURCE_TYPE>(resource);
//	//	auto pAllocatedResource = m_umapResourceSptrDataById[resourceId].get();
//
//	//	// if string supplied, associate string with raw pointer to resource
//	//	if (!stringId.empty()) {
//	//		m_umapResourcePtrByString[stringId] = pAllocatedResource; //m_umapResourceSptrDataById[resourceId].get();			
//	//	}
//
//	//	return { resourceId, pAllocatedResource};
//	//}
//
//
//	TResourceHandle<RESOURCE_TYPE> Add(const std::shared_ptr<RESOURCE_TYPE>& resource, const std::wstring& stringId = "")
//	{
//		auto resourceId = GetNextId();
//		auto& allocatedResource = m_umapResourceSptrDataById[resourceId] = resource;
//
//		// if string supplied, associate string with raw pointer to resource
//		if (!stringId.empty()) {
//			m_umapResourcePtrByString[stringId] = allocatedResource.get(); //m_umapResourceSptrDataById[resourceId].get();			
//		}
//
//		return { resourceId, allocatedResource.get() };
//	}
//
//private:
//	std::map<uint32_t, std::shared_ptr<RESOURCE_TYPE>> m_umapResourceSptrDataById;
//
//	std::unordered_map<
//		std::wstring,
//		RESOURCE_TYPE*,
//		CaseInsensitiveUnorderedMapComparer::Hash,
//		CaseInsensitiveUnorderedMapComparer::Compare> m_umapResourcePtrByString;
//};
//
//
//TResourceManager<DxMesh>* GetMeshes();
//TResourceManager<DxTexture>* GetTextures();
//TResourceManager<DxMaterial>* GetMaterialManager();
//TResourceManager<IDxShaderProgram>* Shaders();
//
//	private:
//		TResourceManager<DxMesh> m_spoMeshManager;
//		TResourceManager<DxTexture> m_spoTextureManager;
//		TResourceManager<DxMaterial> m_spoMaterialManager;
//		TResourceManager<IDxShaderProgram> m_spoShaderManager;
//

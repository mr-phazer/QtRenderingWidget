#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <map>

// author
#include "..\Helpers\NoCaseMapCompare.h"

#include "IDxResouce.h"

namespace Rldx {


	


	// forward declarations
	class DxMeshData;
	class DxTexture;
	class DxMaterial;
	class IDxShaderProgram;


	class ResourceHandle
	{
	public:
		~ResourceHandle() = default;

		ResourceHandle(uint32_t id, IDxResource* ptr) : m_id(id), m_ptr(ptr) {};
		//ResourceHandle(uint32_t id, char* pszStringId, RESOURCE_TYPE* ptr) : m_id(id), m_pszName(pszStringId), m_ptr(ptr) {};

		uint32_t GetId() const { return m_id; };
		IDxResource* GetPtr() const { return m_ptr; };

	private:
		char* m_pszName = nullptr;
		int32_t m_id = -1;
		IDxResource* m_ptr;
	};


	template <typename RESOURCE_TYPE>
	class TResourceHandle
	{
	public:
		~TResourceHandle() = default;

		TResourceHandle(uint32_t id,  RESOURCE_TYPE* ptr) : m_id(id), m_ptr(ptr) {};
		//ResourceHandle(uint32_t id, char* pszStringId, RESOURCE_TYPE* ptr) : m_id(id), m_pszName(pszStringId), m_ptr(ptr) {};

		uint32_t GetId() const { return m_id; };
		RESOURCE_TYPE* GetPtr() const { return m_ptr; };

	private:
		char* m_pszName = nullptr;
		int32_t m_id = -1;
		RESOURCE_TYPE* m_ptr;
	};

	

	// non-template parent, SHOULD? make sure that the static id is unique per instantiation?
	class IdCounter
	{	
	protected:
		uint32_t GetNextId() const { return sm_nextId++; }

	private:
		static uint32_t sm_nextId;		
	};


	class DxResourceManager : public IdCounter
	{	
	public:
		static DxResourceManager* Instance();		

		template <typename TYPE_DERIVED>
		TResourceHandle<TYPE_DERIVED> AddEmpty(const std::string& stringId)
		{
			auto resourceId = GetNextId();
			auto pDerived = new TYPE_DERIVED;
			m_resourceDataById[resourceId] = std::shared_ptr<IDxResource>(pDerived);
			 
			// if string supplied, associate string with raw pointer to resource
			if (!stringId.empty()) {
				m_resourceDataByString[stringId] = pDerived;
			}

			return { resourceId, pDerived };
		}

		template <typename TYPE_DERIVED>
		TYPE_DERIVED* GetResourceByString(const std::string& strId) const
		{	
			auto whatType = TYPE_DERIVED().GetType();
			
			auto it = m_resourceDataByString.find(strId);
			if (it != m_resourceDataByString.end()) {

				return static_cast<TYPE_DERIVED*>(it->second);
			}

			return nullptr;
		};


		template <typename SHADER_TYPE>
		TResourceHandle<SHADER_TYPE> AllocShaderProgram(const std::string& strId = "")
		{
			return AddEmpty<SHADER_TYPE>(strId);
		}



		TResourceHandle<DxTexture> AllocTexture(const std::string& strId = "");
		TResourceHandle<DxTexture> AllocMaterial(const std::string& strId = "");
		TResourceHandle<DxTexture> AllocMesh(const std::string& strId = "");
		
		/*TResourceHandle<IDxShaderProgram> GetShaderProgram(const std::string& strId = "")
		{
			return AddEmpty<IDxShaderProgram>(strId);
		}*/
		
		//TResourceHandle<IDxShaderProgram> GetShaderProgram(uint32_t id)
		//{
		//	//return AddEmpty<IDxShaderProgram>(id);
		//}
		


	private:
		static std::unique_ptr<DxResourceManager> sm_spoInstance;

	private:
		std::map<uint32_t, std::shared_ptr<IDxResource>> m_resourceDataById;

		std::unordered_map<
			std::string,
			IDxResource*,
			CaseInsensitiveUnorderedMapComparer::Hash,
			CaseInsensitiveUnorderedMapComparer::Compare> m_resourceDataByString;
	};

}; // namespace Rldx

// TODO: remove once all usuable code is moved to DxResourceManager
//template <typename RESOURCE_TYPE>
//class TResourceManager : public ResourceManagerBase
//{
//public:
//
//	/*RESOURCE_TYPE* GetResourceById(uint32_t id) const
//	{
//		auto it = m_resourceDataById.find(id);
//		if (it != m_resourceDataById.end())
//		{
//			return it->second.get();
//		}
//
//		return nullptr;
//	};
//
//	RESOURCE_TYPE* GetResourceByString(const std::string& strId) const
//	{
//		auto it = m_resourceDataByString.find(strId);
//		if (it != m_resourceDataByString.end()) {
//			return it->second;
//		}
//
//		return nullptr;
//	};*/
//
//	//template <typename TYPE_DERIVED>
//	//TResourceHandle<TYPE_DERIVED> AddEmpty(const std::string& stringId)
//	//{
//	//	auto resourceId = GetNextId();
//	//	auto pDerived = new TYPE_DERIVED;
//	//	m_resourceDataById[resourceId] = std::shared_ptr<RESOURCE_TYPE>(pDerived);
//	//	auto pAllocatedResource = m_resourceDataById[resourceId].get();
//
//	//	// if string supplied, associate string with raw pointer to resource
//	//	if (!stringId.empty()) {
//	//		m_resourceDataByString[stringId] = pAllocatedResource; //m_resourceDataById[resourceId].get();			
//	//	}			
//
//	//	return { resourceId, pDerived};
//	//}
//
//
//	//template <typename TYPE_DERIVED>
//	//ResourceHandle<TYPE_DERIVED> Add(TYPE_DERIVED* resource, const std::string& stringId = "")
//	//{
//	//	auto resourceId = GetNextId();
//	//	m_resourceDataById[resourceId] = std::shared_ptr<RESOURCE_TYPE>(resource);
//	//	auto pAllocatedResource = m_resourceDataById[resourceId].get();
//
//	//	// if string supplied, associate string with raw pointer to resource
//	//	if (!stringId.empty()) {
//	//		m_resourceDataByString[stringId] = pAllocatedResource; //m_resourceDataById[resourceId].get();			
//	//	}
//
//	//	return { resourceId, pAllocatedResource};
//	//}
//
//
//	TResourceHandle<RESOURCE_TYPE> Add(const std::shared_ptr<RESOURCE_TYPE>& resource, const std::string& stringId = "")
//	{
//		auto resourceId = GetNextId();
//		auto& allocatedResource = m_resourceDataById[resourceId] = resource;
//
//		// if string supplied, associate string with raw pointer to resource
//		if (!stringId.empty()) {
//			m_resourceDataByString[stringId] = allocatedResource.get(); //m_resourceDataById[resourceId].get();			
//		}
//
//		return { resourceId, allocatedResource.get() };
//	}
//
//private:
//	std::map<uint32_t, std::shared_ptr<RESOURCE_TYPE>> m_resourceDataById;
//
//	std::unordered_map<
//		std::string,
//		RESOURCE_TYPE*,
//		CaseInsensitiveUnorderedMapComparer::Hash,
//		CaseInsensitiveUnorderedMapComparer::Compare> m_resourceDataByString;
//};
//
//
//TResourceManager<DxMeshData>* GetMeshes();
//TResourceManager<DxTexture>* GetTextures();
//TResourceManager<DxMaterial>* GetMaterialManager();
//TResourceManager<IDxShaderProgram>* Shaders();
//
//	private:
//		TResourceManager<DxMeshData> m_spoMeshManager;
//		TResourceManager<DxTexture> m_spoTextureManager;
//		TResourceManager<DxMaterial> m_spoMaterialManager;
//		TResourceManager<IDxShaderProgram> m_spoShaderManager;
//

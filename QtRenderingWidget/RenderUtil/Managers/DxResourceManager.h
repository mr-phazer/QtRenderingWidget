#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>


namespace Rldx
{
	
	class DxResourceBase
	{	
	public:
		ResourceTypeEnum m_resourceType = ResourceTypeEnum::Unknown;				
	};

	class DxMeshResource : public DxResourceBase
	{
		DxMeshResource() : DxResourceBase(ResourceTypeEnum::Mesh) {};
	};	


	class Texture;
	class Shader
	{
		enum ShaderTypesEnum : uint32_t
		{
			PixelShader = 0,
			VertexShader = 1
		};
	};


	// add a next-id to class
	template <typename RESOURCE_TYPE, typename ID_TYPE>
	class TResourceManagerCustomId
	{
		RESOURCE_TYPE* GetResourceById(const ID_TYPE& id) const
		{
			auto it = m_resourceData.find(id);
			if (it != m_resourceData.end())
			{
				return it->second.get();
			}

			return nullptr;
		}

		void AddResourceById(const ID_TYPE& id, std::unique_ptr<RESOURCE_TYPE> resource)
		{
			m_resourceData[id] = std::move(resource);
		}


	private:
		std::unordered_map<ID_TYPE, std::unique_ptr<RESOURCE_TYPE>> m_resourceData;
	};

	using ResId = uint32_t;

	// non-template parent, SHOULD? make sure that the static id is unique per instantiation?
	class IntAutoKeyBase
	{
	protected:
		ResId GetNextId() const { return sm_nextId++; }

	private:
		static ResId sm_nextId;
	};

	template <typename RESOURCE_TYPE>
	class TResourceManagerIntKey : public IntAutoKeyBase
	{
	public:
		RESOURCE_TYPE* GetResourceById(ResId id) const
		{
			auto it = m_resourceData.find(id);
			if (it != m_resourceData.end())
			{
				return it->second.get();
			}

			return nullptr;
		}

		ResId AddResource(std::unique_ptr<RESOURCE_TYPE>& resource)
		{
			auto newId = GetNextId();
			m_resourceData[newId] = std::move(resource);

			return newId;
		}

	private:
		std::unordered_map<ResId, std::unique_ptr<RESOURCE_TYPE>> m_resourceData;
	};


	enum class ManagerEnum : uint32_t
	{
		MeshManager,
		MaterialManager,
		TextureManager,
	};

	class Material
	{
		int i = 0;
	};

	// TODO: use mesh buffer code from qtRME, refactor it nicely
	class Mesh;


	class Texture;

	// make a singleton
	class ResourceManager
	{
	public:
		ResId AddMaterial(std::unique_ptr<Material> material)
		{
			return m_materialManager.AddResource(material);
		}

		Material* GetMaterialById(ResId id) const
		{
			return m_materialManager.GetResourceById(id);
		}

		ResId AddMesh(std::unique_ptr<Mesh>& mesh)
		{
			return m_meshManager.AddResource(mesh);
		}

		static ResourceManager& getInstance()
		{
			if (!m_spuInstance)
			{
				m_spuInstance = std::make_unique<ResourceManager>();
			}

			return *m_spuInstance;
		}

	private:
		TResourceManagerIntKey<Mesh> m_meshManager;
		TResourceManagerIntKey<Material> m_materialManager;

		struct PrivateTextureManager
		{
			TResourceManagerCustomId<std::string, std::shared_ptr<Texture>> m_textureManagerString;
			TResourceManagerIntKey<std::shared_ptr<Texture>> m_textureMangerInt;
		}
		textureManager;

	private:

		static std::unique_ptr<ResourceManager> m_spuInstance;
	};
};

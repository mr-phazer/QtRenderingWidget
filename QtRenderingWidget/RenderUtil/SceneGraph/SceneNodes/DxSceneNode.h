#pragma once


#include <vector>
#include <string>
#include <memory>
#include "..\NodeTransform\NodeTransform.h"
#include "..\..\Managers\DxResourceManager.h"
#include "..\..\Managers\DxTypes.h"


namespace Rldx {

	class Identifiable
	{	
	public:
		Identifiable() : m_id(GetNextId()) {}
		uint32_t GetId() const { return m_id; }		
	
	public:
		static constexpr uint32_t INVALID_ID = ~0;

	private:
		static uint32_t GetNextId() { return sm_nextId++; }

	private:
		uint32_t m_id = INVALID_ID;
		static uint32_t sm_nextId;
	};

	class DxSceneNode : public Identifiable
	{
	public:
		using Sptr = std::shared_ptr<DxSceneNode>;
	public:		
		virtual ~DxSceneNode();

		static DxSceneNode::Sptr Create(std::string name = "");

		ResourceTypeEnum GetResourceType();
		void SetResource(ResId resourceId) { resourceId = resourceId; };
		ResId GetResurce() const { return m_resourceId; };

		DxSceneNode* GetParent();

		void AddChild(DxSceneNode::Sptr& spoChild);
		//void AddChild(DxSceneNode* poChild); // TODO: should remove? require user always use shared_ptr
		DxSceneNode* GetChildByPtr(DxSceneNode* poChild);
		DxSceneNode* GetChild(size_t index = 0);

		size_t GetChildCount() const;

		// TODO: test this
		static DxSceneNode* FindChild(DxSceneNode* nodeToFind, DxSceneNode* currentNode);

		const std::vector<DxSceneNode::Sptr>& GetChildren() const;
		std::vector<DxSceneNode::Sptr>& GetChildren();

		void RemoveChild(const DxSceneNode::Sptr& spoChild);
		void RemoveChild(DxSceneNode* poChild);
		void RemoveChild(size_t index);
		void RemoveChildren();

		NodeTransform& GetTransform();
		const NodeTransform& GetTransform() const;

	private:
		void SetParent(DxSceneNode* poParent);

	protected:
		// node type:
		ResourceTypeEnum m_resourceType = ResourceTypeEnum::Unknown;
		ResId m_resourceId = 0;

	private:
		// tree structure
		std::vector<Sptr> m_children;
		DxSceneNode* m_wpoParent = nullptr;
	private:
		// geometruy	
		NodeTransform m_nodeTransform;
	private:
		int m_resouceType = 0;
		std::string m_name = "SceneNode";


		// node ids	
		//NodeId m_nodeId = ~0;
		//static NodeId sm_nextId;		

	};


}; // namespace Rldx
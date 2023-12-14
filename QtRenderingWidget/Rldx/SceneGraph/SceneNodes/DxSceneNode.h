#pragma once


#include <vector>
#include <string>
#include <memory>
#include "..\NodeTransform\NodeTransform.h"
#include "..\..\Managers\ResourceManager\DxResourceManager.h"
#include "..\..\Managers\DxTypes.h"


namespace Rldx {
	
	using IntId = uint32_t;
	static const IntId INVALID_ID = ~0;

	class Identifiable
	{			
	public:		
		Identifiable(const std::string& name) : m_id(GetNextId()), m_name(name) {}
		Identifiable() : m_id(GetNextId()) {}
		IntId GetId() const { return m_id; }
		std::string GetName() const { return m_name; }
		virtual std::string GetTypeString() = 0;

	private:
		static IntId GetNextId() { return sm_nextId++; }

	private:
		IntId m_id = INVALID_ID;
		static IntId sm_nextId;

	protected:
		std::string m_name = "Unnamed_Object";
	};

	class DxSceneNode : public Identifiable
	{
	public:
		using Sptr = std::shared_ptr<DxSceneNode>;
	public:		
		DxSceneNode() = default;
		virtual ~DxSceneNode();
		DxSceneNode(const std::string& name) { SetName(name); };

		static DxSceneNode::Sptr Create(std::string name = "");

		void SetName(const std::string& name);

		ResourceTypeEnum GetResourceType();		
		virtual std::string GetTypeString() override { return "DxSceneNode"; }

		DxSceneNode* GetParent();

		void AddChild(DxSceneNode::Sptr spoChild);
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
		uint32_t m_resourceId = 0;

	private:
		// tree structure
		std::vector<Sptr> m_children;
		DxSceneNode* m_wpoParent = nullptr;
	private:
		// geometruy	
		NodeTransform m_nodeTransform;
	private:
		int m_resouceType = 0;

		// node ids	
		//NodeId m_nodeId = ~0;
		//static NodeId sm_nextId;		
		
	};


}; // namespace Rldx
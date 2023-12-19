#pragma once


#include <vector>
#include <string>
#include <memory>
#include "..\NodeTransform\NodeTransform.h"
#include "..\..\Managers\ResourceManager\DxResourceManager.h"
#include "..\..\Managers\DxTypes.h"
#include "..\Interfaces\IIdentifiable.h"


namespace Rldx {
	class DxSceneNode : public IIdentifiable
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
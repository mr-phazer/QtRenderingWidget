#pragma once


#include <vector>
#include <string>
#include "..\NodeTransform\NodeTransform.h"
#include "..\..\Managers\DxResourceManager.h"
#include "..\..\Managers\DxTypes.h"
#include <memory>

namespace Rldx {
		
	class DxSceneNode : public std::enable_shared_from_this<DxSceneNode>
	{
	public:
		using SPtr = std::shared_ptr<DxSceneNode>;	
	public:										
		virtual ~DxSceneNode();			

		static DxSceneNode::SPtr Create(std::string name = "");

		ResourceTypeEnum GetResourceType();
		void SetResource(ResId resourceId) { resourceId = resourceId; };
		ResId GetResurce() const { return m_resourceId; };

		void SetParent(DxSceneNode::SPtr parent);
		DxSceneNode* GetParent();

		void AddChild(DxSceneNode::SPtr child);
		void AddChild(DxSceneNode* poChild);		
		DxSceneNode* GetChild(DxSceneNode* poChild);
		DxSceneNode* GetChild(size_t index=0);
		
		// TODO: test this
		static DxSceneNode* FindNode(DxSceneNode* nodeToFind, DxSceneNode* currentNode);

		const std::vector<DxSceneNode::SPtr>& GetChildren() const;
		std::vector<DxSceneNode::SPtr>& GetChildren();

		void RemoveChild(DxSceneNode::SPtr spoChild);
		void RemoveChild(DxSceneNode* poChild);
		void RemoveChild(size_t index);
		void RemoveChildren();

		NodeTransform& GetTransform();
		const NodeTransform& GetTransform() const;

		NodeId GetId();

	private:
		static NodeId GetNextId();

	protected:
		// node type:
		ResourceTypeEnum m_resourceType = ResourceTypeEnum::Unknown;
		ResId m_resourceId = 0;	

	private:
		// tree structure
		std::vector<SPtr> m_children;
		std::weak_ptr<DxSceneNode> m_wpoParent;
	private:
		// geometruy	
		NodeTransform m_nodeTransform;
	private:
		// node ids	
		NodeId m_nodeId = ~0;
		std::string m_name = "SceneNode";
		static NodeId sm_nextId;	
	};		

	class DxMeshNode : public DxSceneNode
	{		
		DxMeshNode() : DxSceneNode() { m_resourceType = ResourceTypeEnum::Mesh; };

		void SetMesh(Mesh* meshData)
		{
			
		
		}

		// TODO: use mesh buffer code from qtRME, refactor it nicely
		Mesh* meshData; 
	};

}; // namespace Rldx
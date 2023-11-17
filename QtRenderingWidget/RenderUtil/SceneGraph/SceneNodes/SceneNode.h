#pragma once


#include <vector>
#include <string>
#include "..\NodeTransform\NodeTransform.h"
#include "..\..\Managers\DxResourceManager.h"
#include "..\..\Managers\DxTypes.h"
#include <memory>

namespace Rldx {
		
	class SceneNode : public std::enable_shared_from_this<SceneNode>
	{
	public:
		using SPtr = std::shared_ptr<SceneNode>;	
	public:										
		virtual ~SceneNode();			

		static SceneNode::SPtr Create(std::string name = "");

		ResourceTypeEnum GetResourceType();
		void SetResource(ResId resourceId) { resourceId = resourceId; };
		ResId GetResurce() const { return m_resourceId; };

		void SetParent(SceneNode::SPtr parent);
		SceneNode* GetParent();

		void AddChild(SceneNode::SPtr child);
		void AddChild(SceneNode* poChild);		
		SceneNode* GetChild(SceneNode* poChild);
		SceneNode* GetChild(size_t index=0);
		
		// TODO: test this
		static SceneNode* FindNode(SceneNode* nodeToFind, SceneNode* currentNode);

		const std::vector<SceneNode::SPtr>& GetChildren() const;
		std::vector<SceneNode::SPtr>& GetChildren();

		void RemoveChild(SceneNode::SPtr spoChild);
		void RemoveChild(SceneNode* poChild);
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
		std::weak_ptr<SceneNode> m_wpoParent;

		// geometruy	
		NodeTransform m_nodeTransform;

		// node ids	
		NodeId m_nodeId = ~0;
		std::string m_name = "SceneNode";
		static NodeId sm_nextId;	
	};		

	class DxMeshNode : public SceneNode
	{		
		DxMeshNode() : SceneNode() { m_resourceType = ResourceTypeEnum::Mesh; };

		void SetMesh(Mesh* meshData)
		{
			
		
		}

		// TODO: use mesh buffer code from qtRME, refactor it nicely
		Mesh* meshData; 
	};

}; // namespace Rldx
#pragma once


#include <vector>
#include <string>
#include <memory>
#include "..\NodeTransform\NodeTransform.h"
#include "..\..\Managers\ResourceManager\DxResourceManager.h"
#include "..\..\Managers\DxTypes.h"
#include "..\Interfaces\IIdentifiable.h"
#include "..\Rldx\Managers\ResourceManager\IDxResouce.h"

namespace Rldx {

	enum SceneNodeTypeEnum : uint32_t
	{
		Unknown,
		EmptyNode,
		Texture,
		Shader,
		Mesh,
		Material
	};

	class IDxSceneNode : public TIdentifiable<SceneNodeTypeEnum>
	{
	public:
		using Sptr = std::shared_ptr<IDxSceneNode>;
	public:		
		IDxSceneNode() = default;
		virtual ~IDxSceneNode();
		IDxSceneNode(const std::string& name) { SetName(name); };

		IDxSceneNode* GetParent();

		void AddChild(IDxSceneNode::Sptr spoChild);
		//void AddChild(IDxSceneNode* poChild); // TODO: should remove? require user always use shared_ptr
		IDxSceneNode* GetChildByPtr(IDxSceneNode* poChild);
		IDxSceneNode* GetChild(size_t index = 0);

		size_t GetChildCount() const;

		// TODO: test this
		static IDxSceneNode* FindChild(IDxSceneNode* nodeToFind, IDxSceneNode* currentNode);


		const std::vector<IDxSceneNode::Sptr>& GetChildren() const;
		std::vector<IDxSceneNode::Sptr>& GetChildren();

		void RemoveChild(const IDxSceneNode::Sptr& spoChild);
		void RemoveChild(IDxSceneNode* poChild);
		void RemoveChild(size_t index);
		void RemoveChildren();

		NodeTransform& GetTransform();
		const NodeTransform& GetTransform() const;

	private:
		void SetParent(IDxSceneNode* poParent);

	protected:
		// node type:
		//SceneNodeTypeEnum m_resourceType = SceneNodeTypeEnum::Unknown;
		//uint32_t m_resourceId = 0;

	private:
		// -- tree structure
		std::vector<Sptr> m_children;
		IDxSceneNode* m_wpoParent = nullptr;
	private:	
		// -- Node geometry, translation, rotation,s cale
		NodeTransform m_nodeTransform;		
	};

	class DxSceneNode : public IDxSceneNode
	{	
	public:
		static IDxSceneNode::Sptr Create(std::string name = "");

		std::string GetTypeString() const override { return "DxSceneNode"; };
		SceneNodeTypeEnum GetType() const { return SceneNodeTypeEnum::EmptyNode; };
	};

}; // namespace Rldx
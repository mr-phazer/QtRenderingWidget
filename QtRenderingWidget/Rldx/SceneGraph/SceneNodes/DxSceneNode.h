//#pragma once
//
//
//#include <vector>
//#include <string>
//#include <memory>
//#include "..\NodeTransform\NodeTransform.h"
//#include "..\..\Managers\ResourceManager\DxResourceManager.h"
//#include "..\..\Managers\DxTypes.h"
//#include "..\Interfaces\IIdentifiable.h"
//#include "..\Interfaces\IUpdateable.h"
//#include "..\Interfaces\IFlushAble.h"
//#include "..\rldx\Managers\ResourceManager\IDxResouce.h"
//#include "..\..\SceneGraph\BaseNode\DxBaseNode.h"
//
//namespace rldx {
//		
//	class DxSceneNode : public DxBaseNode
//	{	
//	public:
//		using SharedPointerBase = std::shared_ptr<DxSceneNode>;
//	public:
//		static SharedPointerBase Create(std::string name = "");
//
//		std::string GetTypeString() const override { return "DxSceneNode"; };
//		SceneNodeTypeEnum GetType() const { return SceneNodeTypeEnum::EmptyNode; };
//
//		void Update(float timeElapsed) override;
//		void FlushToRenderQueue(IRenderBucket* pRenderQueue) override;
//	};
//
//	
//
//}; // namespace rldx
//
//
//
////template <typename NODE_TYPE>
////class IDxSceneNode : public ITDxTreeNode<I
////{
////public:
////	using SharedPointerBase = std::shared_ptr<IDxSceneNode>;
////public:		
////	IDxSceneNode() = default;
////	virtual ~IDxSceneNode();		
//
////	virtual void SetName(const std::string& name);
//
////	NODE_TYPE* GetParent();
//
////	void AddChild(IDxSceneNode::SharedPointerBase spoChild);
////	//void AddChild(IDxSceneNode* poChild); // TODO: should remove? require user always use shared_ptr
////	NODE_TYPE* GetChildByPtr(IDxSceneNode* poChild);
////	NODE_TYPE* GetChild(size_t index = 0);
//
////	template <typename T, bool bDynamicCasting = true>
////	inline T* GetThisAs();	
//
////	size_t GetChildCount() const;
//
////	// TODO: test this
////	static NODE_TYPE* FindChild(IDxSceneNode* nodeToFind, IDxSceneNode* currentNode);
//
////	const std::vector<NODE_TYPE::SharedPointerBase>& GetChildren() const;
////	std::vector<NODE_TYPE::SharedPointerBase>& GetChildren();
//
////	void RemoveChild(const IDxSceneNode::SharedPointerBase& spoChild);
////	void RemoveChild(IDxSceneNode* poChild);
////	void RemoveChild(size_t index);
////	void RemoveChildren();		
////	
////private:
////	void SetParent(IDxSceneNode* poParent);
//
////protected:
////	// node type:
////	//SceneNodeTypeEnum m_resourceType = SceneNodeTypeEnum::Unknown;
////	//uint32_t m_resourceId = 0;
//
////};
//
//

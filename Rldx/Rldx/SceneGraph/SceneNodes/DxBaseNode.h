#pragma once

#include <stack>
#include <vector>

#include "..\..\Interfaces\IFlushable.h"
#include "..\..\Interfaces\IUpdateable.h"
#include "..\..\Interfaces\TIdentifiable.h"

#include "..\..\Rendering\DxMeshRenderData.h"
#include "..\NodeTransform\NodeTransform.h"

namespace rldx {

	enum class SceneNodeTypeEnum : uint32_t
	{
		Unknown,
		EmptyNode,
		BaseNode,
		MeshNode,
		ModelNode,
		VmdNode,
	};

	class IRenderBucket;
	class DxDeformerNode; // forward decl, as DxDeformerNode is derived DxBaseNode


	class DxNodeCreator
	{
	public:
		template<typename NODE_TYPE>
		static std::shared_ptr<NODE_TYPE> CreateNode(const std::wstring& name = "")
		{
			auto newInstance = std::make_shared<NODE_TYPE>();
			newInstance->SetName(name);
			return newInstance;
		}
	};


	class DxBaseNode : public TIdentifiable<SceneNodeTypeEnum>, public IUpdateable/*, public IFlushable*/
	{
		DxMeshRenderingData m_boundingBoxMesh;
		DxMeshRenderingData m_nodeMesh;

	public:
		enum class DrawStateEnum : bool { DontDraw = false, Draw = true };
		using SharedPtr = std::shared_ptr<DxBaseNode>;

	private:
		// Tree structure 
		std::vector<SharedPtr> m_children;
		DxBaseNode* m_wpoParent = nullptr;

		// Node geomtry
		NodeTransform m_nodeTransform;
		sm::Matrix m_tempGlobalTransForm = sm::Matrix::Identity;

		// TODO: check up on the init requiremnt
		// init to very small extend, but not zero, as it will/might cause problems with bounding box
		DirectX::BoundingBox m_BoundBox = DirectX::BoundingBox({ 0,0,0 }, { 0E-7, 0E-7, 0E-7 });

		DrawStateEnum m_drawState = DrawStateEnum::Draw;
		DrawStateEnum m_BoundingBoxDrawState = DrawStateEnum::DontDraw;

	public:
		DxBaseNode() : TIdentifiable<SceneNodeTypeEnum>(L"Unames DxBaseNode") {}
		DxBaseNode(const std::wstring& name) : TIdentifiable<SceneNodeTypeEnum>(name) {}
		virtual ~DxBaseNode() = default;


	public:
		// TODO: make constructor, to force derived classes to set NodeType Enum / Type Description String
		// TODO: make: "DxBaseNode(SceneNodeTypeEnum type, std::wstring Name)

		DirectX::BoundingBox& GetNodeBoundingBox() { return m_BoundBox; }

		void SetBoundingBox(DirectX::XMFLOAT3 minPoint, DirectX::XMFLOAT3 maxPoint);

		void SetBoundingBox(const DirectX::BoundingBox& inBB);

		virtual void SetDeformerNode(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex)
		{
			auto debug_break_1 = 1;;
		};

		virtual void SetDeformerNodeRecursive(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex);

		virtual void SetAttachBone(int32_t boneIndex) {};

		static SharedPtr Create(const std::wstring& name = L"")
		{
			auto newInstance = std::make_shared<DxBaseNode>();
			newInstance->SetName(name);
			return newInstance;
		}

		virtual void SetName(const std::wstring& strName)
		{
			name = this->GetTypeString() + L" # " + strName + L" # " + std::to_wstring(GetId());
		}

		std::wstring GetName() const
		{
			return name;
		}

		DxBaseNode* GetParent()
		{
			return m_wpoParent;
		}

		const DxBaseNode* GetParent() const
		{
			return m_wpoParent;
		}

		size_t GetChildCount() const
		{
			return m_children.size();
		}

		void AddChild(SharedPtr spoChild)
		{
			if (spoChild == nullptr)
			{
				return;
			}

			spoChild->SetParent(this);
			m_children.push_back(spoChild);
		}

		DxBaseNode* GetChildByPtr(DxBaseNode* poChild)
		{
			for (size_t i = 0; i < m_children.size(); i++)
			{
				if (m_children[i].get() == poChild)
				{
					return m_children[i].get();
				}
			}

			return nullptr;
		}

		DxBaseNode* GetChild(size_t index = 0)
		{
			if (index >= m_children.size())
			{
				return nullptr;
			}

			return m_children[index].get();
		}

		template<typename CONST_BUF_DATA_TYPE, bool bDynamicCasting = true>
		inline CONST_BUF_DATA_TYPE* GetThisAs()
		{
			if (bDynamicCasting)
			{
				return dynamic_cast<CONST_BUF_DATA_TYPE*>(this);
			}
			else
			{
				return static_cast<CONST_BUF_DATA_TYPE*>(this);
			}
		}

		// TODO: tesTreeNodet this
		static DxBaseNode* FindChild(DxBaseNode* nodeToFind, DxBaseNode* currentNode)
		{
			// to break out of recursion
			if (currentNode == nodeToFind) {
				return currentNode;
			}

			for (auto& childNode : currentNode->GetChildren())
			{
				auto hrResult = FindChild(nodeToFind, childNode.get());

				if (hrResult != nullptr)
					return hrResult;
			}

			return nullptr;
		};

		const std::vector<SharedPtr>& GetChildren() const
		{
			return m_children;
		}

		std::vector<SharedPtr>& GetChildren()
		{
			return m_children;
		}

		void RemoveChild(const SharedPtr& spoChild)
		{
			for (size_t i = 0; i < m_children.size(); i++)
			{
				if (m_children[i].get() == spoChild.get())
				{
					RemoveChild(i);
					break;
				}
			}

		};

		void RemoveChild(DxBaseNode* poChild)
		{
			for (size_t i = 0; i < m_children.size(); i++)
			{
				if (m_children[i].get() == poChild)
				{
					RemoveChild(i);
					break;
				}
			}
		}

		void RemoveChild(size_t index)
		{
			if (m_children.size() > index)
			{
				m_children.erase(m_children.begin() + index);
			}
		}

		// TODO: MAKE WORK
		template <typename NODE_TYPE, typename WORK_TYPE>
		static void DoTreeWOrk(NODE_TYPE* node, const WORK_TYPE&& work);


		void RemoveChildren()
		{
			m_children.clear();
		}

		NodeTransform& Transform() { return m_nodeTransform; };
		const NodeTransform& Transform() const { return m_nodeTransform; };

		DxBaseNode* GetTreeRootNode()
		{
			auto poCurrentNode = this;

			if (poCurrentNode == nullptr)
				throw std::exception("Fatal Error: this == nullptr");

			// get parent of current node
			auto parentNode = poCurrentNode->GetParent();
			while (parentNode)
			{
				// go a level up the tree
				poCurrentNode = parentNode;
				parentNode = poCurrentNode->GetParent();
			}

			return poCurrentNode;
		}

		virtual void UpdateAllBoundBoxes()
		{
			auto poRootNode = GetTreeRootNode();

			if (poRootNode == nullptr) return
				UpdateBoundBoxesRecursive(poRootNode);
		}

		virtual void FitChildBoundBox(DxBaseNode* node)
		{
			DirectX::BoundingBox::CreateMerged(
				this->GetNodeBoundingBox(),
				this->GetNodeBoundingBox(),
				node->GetNodeBoundingBox());
		}

		virtual void UpdateBoundBoxesRecursive(DxBaseNode* node)
		{
			FitChildBoundBox(node);

			for (auto& itChild : node->GetChildren())
			{
				UpdateBoundBoxesRecursive(itChild.get());
			}
		}

		void UpdateGlobalTransform(float elapsetTime = 0.0f)
		{
			auto parent = GetParent();

			m_tempGlobalTransForm = parent ?
				m_nodeTransform.GetGlobalTransform(parent->GetCurrentGlobalTransForm())
				:
				m_nodeTransform.LocalTransform();
		}

		sm::Matrix GetCurrentGlobalTransForm() const
		{
			return m_tempGlobalTransForm;
		}

		virtual void Update(float timeElapsed) override;
		virtual void FlushToRenderBucket(IRenderBucket* pRenderQueue);

		virtual void SetDrawState(DrawStateEnum state) { m_drawState = state; }
		DrawStateEnum GetDrawState() const { return m_drawState; }

	private:
		void SetParent(DxBaseNode* poParent)
		{
			m_wpoParent = poParent;
		}

	private:
		std::wstring GetTypeString() const override;
		SceneNodeTypeEnum GetType() const override;
	};

	template<typename NODE_TYPE, typename WORK_TYPE>
	inline void DxBaseNode::DoTreeWOrk(NODE_TYPE* rootNode, const WORK_TYPE&& WorkFunc)
	{
		//std::vector<NODE_TYPE*> nodeStack;
		//nodeStack.push_back(poNode);

		//while (nodeStack.size() > 0)
		//{
		//	NODE_TYPE* node = nodeStack.back(); // get copy of last element (raw pointers)
		//	nodeStack.pop_back(); // remove that last element from "stack

		//	WorkFunc(node);

		//	for (auto& itSpNode : node->GetChildren()) {
		//		nodeStack.push_back(itSpNode.get());
		//	}
		//}
	}


}; // namespace rldx

/// <summary>
/// Work can do stuff, as add to queue, update stuff, and other thing (member params set in constrcutor)
/// </summary>
//class INodeWorker
//{
//public:
//	virtual void DoWork(TreeNode*) = 0;
//};

//class TreeNode
//{
//public:
//	/// <summary>
//	/// Recursiveky go trough the tree (sceneGraph)
//	/// for ecah node, all "worker" are "applied"
//	/// </summary>
//	void ForEachNode(std::vector<INodeWorker*> nodeWorkes);
//};


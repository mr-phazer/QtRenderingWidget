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
		VariantMeshNode,
		DeformerNode
	};

	// forward decl
	class IRenderBucket;
	class DxDeformerNode;
	class DxVmdNode;
	class DxVariantMeshNode;
	class DxModelNode;

	class DxNodeCreator
	{
	public:
		template<typename NODE_TYPE>
		static std::shared_ptr<NODE_TYPE> CreateNode(const std::wstring& m_nodeName = "")
		{
			auto newInstance = std::make_shared<NODE_TYPE>();
			newInstance->SetName(m_nodeName);
			return newInstance;
		}
	};


	class DxBaseNode : public TIdentifiable<SceneNodeTypeEnum>, public IUpdateable/*, public IFlushable*/
	{
	public:
		enum class DrawStateEnum : bool { DontDraw = false, Draw = true };
		using UniquePtr = std::unique_ptr<DxBaseNode>;

	private:
		DrawStateEnum m_drawState = DrawStateEnum::Draw;
		DrawStateEnum m_BoundingBoxDrawState = DrawStateEnum::Draw;

		DxMeshRenderingData m_boundingBoxMesh;
		DxMeshRenderingData m_nodeMesh;

		// Tree structure 
		std::vector<std::unique_ptr<DxBaseNode>> m_children;
		DxBaseNode* m_wpoParent = nullptr;

		// Node geomtry
		NodeTransform m_nodeTransform;
		sm::Matrix m_tempGlobalTransForm = sm::Matrix::Identity;

		// TODO: check up on the init requiremnt
		// init to very small extend, but not zero, as it will/might cause problems with bounding box
		DirectX::BoundingBox m_BoundBox = DirectX::BoundingBox({ 0,0,0 }, { 0E-7, 0E-7, 0E-7 });

	public:
		DxBaseNode()
		{
			SetType(SceneNodeTypeEnum::MeshNode);
			SetTypeString(L"DxMeshNode");
		}

		DxBaseNode(const std::wstring& name)
		{
			DxBaseNode();
			SetName(name);
		}

		virtual ~DxBaseNode() = default;

		template<typename T>
		static std::unique_ptr<T> TCreateTreeNode(std::wstring nodeName);

	public:
		// TODO: make constructor, to force derived classes to set NodeType Enum / Type Description String
		// TODO: make: "DxBaseNode(SceneNodeTypeEnum type, std::wstring Name)

		DirectX::BoundingBox& GetNodeBoundingBox();

		void SetBoundingBox(DirectX::XMFLOAT3 minPoint, DirectX::XMFLOAT3 maxPoint);
		void SetBoundingBox(const DirectX::BoundingBox& inBB);

		virtual void SetDeformerNode(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex);;
		virtual void SetDeformerNodeRecursive(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex);

		virtual void SetAttachBone(int32_t boneIndex) {};
		static UniquePtr Create(const std::wstring& m_nodeName = L"");


		// TODOD: should m_nodeName be here, when it derived from TIdentifiable that also has m_nodeName?
		std::wstring GetName() const;

		DxBaseNode* GetParent();
		const DxBaseNode* GetParent() const;

		size_t GetChildCount() const;

		// TODO: check that this shit actually work, calling the over overload and not itself
		/*template <typename T>
		T* AddChild(std::unique_ptr<T>&& spoChild)
		{
			return AddChild(static_cast<std::unique_ptr<T>&>(spoChild));
		}*/


		/// <summary>
		/// Adds a child to tree, takes ownership of incoming node, previous instances will be invalid
		/// </summary>
		/// <typeparam name="T">node type</typeparam>
		/// <param name="spoNode">node, unique ptr by value</param>
		/// <returns>non-owning reference to the add child</returns>
		template <typename T>
		T& AddChild(std::unique_ptr<T> spoNode)
		{
			if (!spoNode)
			{
				throw std::exception("Fatal Error: spoChild == nullptr");
			}

			spoNode->SetParent(this);
			m_children.push_back(std::move(spoNode));

			return *static_cast<T*>(m_children.back().get());
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

		const std::vector<UniquePtr>& GetChildren() const
		{
			return m_children;
		}

		std::vector<UniquePtr>& GetChildren()
		{
			return m_children;
		}

		void RemoveChild(const UniquePtr& spoChild)
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

		void RemoveThis();
		void RemoveNode(DxBaseNode* poChild);

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

		static void RemoveChildrenRecursive(DxBaseNode* node)
		{
			if (!node) return;

			for (auto& itChild : node->GetChildren())
			{
				RemoveChildrenRecursive(itChild.get());
			}

			node->RemoveChildren();
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
	};

	template<typename T>
	inline std::unique_ptr<T> DxBaseNode::TCreateTreeNode(const std::wstring nodeName)
	{
		SceneNodeTypeEnum type = SceneNodeTypeEnum::Unknown;

		// check for supported types at compile-time
		static_assert(
			std::is_same<T, DxBaseNode>::value ||
			std::is_same<T, DxMeshNode>::value ||
			std::is_same<T, DxModelNode>::value ||
			std::is_same<T, DxVmdNode>::value ||
			std::is_same<T, DxVariantMeshNode>::value,
			"Fatal Error: Unknown type");

		// assign all the different type enum value based on "is_same" checks
		if (std::is_same<T, DxBaseNode>::value)	type = SceneNodeTypeEnum::BaseNode;
		if (std::is_same<T, DxMeshNode>::value)	type = SceneNodeTypeEnum::MeshNode;
		if (std::is_same<T, DxModelNode>::value)	type = SceneNodeTypeEnum::ModelNode;
		if (std::is_same<T, DxVmdNode>::value)	type = SceneNodeTypeEnum::VmdNode;
		if (std::is_same<T, DxVariantMeshNode>::value)	type = SceneNodeTypeEnum::VariantMeshNode;

		auto newNode = std::make_unique<T>(nodeName);

		return newNode;
	}

}; // namespace rldx
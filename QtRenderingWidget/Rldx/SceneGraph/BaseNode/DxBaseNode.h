#pragma once

#pragma once

#include "..\Rldx\Interfaces\TIdentifiable.h"
#include "..\Rldx\Interfaces\IUpdateable.h"
#include "..\Rldx\Interfaces\IFlushable.h"

#include "..\..\SceneGraph\NodeTransform\NodeTransform.h"

namespace Rldx {

	enum class SceneNodeTypeEnum : uint32_t
	{
		Unknown,
		EmptyNode,		
		BaseNode,
		MeshNode,
	};

	class DxBaseNode : public TIdentifiable<SceneNodeTypeEnum>, public IUpdateable, public IFlushable
	{
	public:
		using SharedPointerBase = std::shared_ptr<DxBaseNode>;

	public:
		DxBaseNode() = default;
		virtual ~DxBaseNode() = default;

		static SharedPointerBase Create(const std::string& name = "")
		{
			auto newMeshNode = std::make_shared<DxBaseNode>();
			newMeshNode->SetName(name);
			return newMeshNode;
		}

		virtual void SetName(const std::string& name)
		{
			m_name = this->GetTypeString() + " # " + name + " # " + std::to_string(GetId());
		}

		DxBaseNode* GetParent()
		{
			return m_wpoParent;
		}

		size_t GetChildCount() const
		{
			return m_children.size();
		}

		void AddChild(SharedPointerBase spoChild)
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

		template<typename T, bool bDynamicCasting = true>
		inline T* GetThisAs()
		{
			if (bDynamicCasting)
			{
				return dynamic_cast<T*>(this);
			}
			else
			{
				return static_cast<T*>(this);
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
				auto result = FindChild(nodeToFind, childNode.get());

				if (result != nullptr)
					return result;
			}

			return nullptr;
		};

		const std::vector<SharedPointerBase>& GetChildren() const
		{
			return m_children;
		}

		std::vector<SharedPointerBase>& GetChildren()
		{
			return m_children;
		}

		void RemoveChild(const SharedPointerBase& spoChild)
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
			for (size_t i = 0; i < m_children.size(); i++)
			{
				if (i == index)
				{
					RemoveChild(i);
					break;
				}
			}
		}

		void RemoveChildren()
		{
			m_children.clear();
		}

		NodeTransform& GetTransform() { return m_nodeTransform; };
		const NodeTransform& GetTransform() const { return m_nodeTransform; };

		void UppdateGlobalTransform(float elapsetTime = 0.0f)
		{
			auto parent = GetParent();

			m_tempGlobalTransForm = parent ?
				m_nodeTransform.GetGlobalTransform(parent->GetCurrentGlobalTransForm())
				:
				m_nodeTransform.GetLocalTransform();
		}

		sm::Matrix GetCurrentGlobalTransForm() const
		{
			return m_tempGlobalTransForm;
		}
				
		void Update(float timeElapsed) override;		
		void FlushToRenderQueue(IRenderBucket* pRenderQueue) override;

	private:
		void SetParent(DxBaseNode* poParent)
		{
			m_wpoParent = poParent;
		}

	private:
		// -- tree structure
		std::vector<SharedPointerBase> m_children;
		DxBaseNode* m_wpoParent = nullptr;
		// -- node geomtry
		NodeTransform m_nodeTransform;
		sm::Matrix m_tempGlobalTransForm = sm::Matrix::Identity;


		// Inherited via TIdentifiable
		std::string GetTypeString() const override;

		SceneNodeTypeEnum GetType() const override;


	

	};
}; // namespace Rldx

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

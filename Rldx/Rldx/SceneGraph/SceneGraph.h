#pragma once
//#include "..\Rendering\DxMesh.h"
#include "..\Rendering\DxShaderProgram.h"
//#include "..\Interfaces\IDrawable.h"
//#include "..\Interfaces\IFlushable.h"
#include "..\Interfaces\IRenderBucket.h"
//#include "..\Interfaces\IUpdateable.h"
//#include "..\Managers\DxDeviceManager.h"
#include "SceneNodes\DxBaseNode.h"

namespace rldx {

	class DxMeshRenderBucket : public IRenderBucket
	{
		std::vector<IRenderQueueItem*> m_renderItems;


	public:
		virtual void AddItem(IRenderQueueItem* renderItem) override { m_renderItems.push_back(renderItem); };

		virtual void ClearItems() override { m_renderItems.clear(); };

		void Draw(ID3D11DeviceContext* poDC, DxMeshShaderProgram* defaultShaderProgram = nullptr)
		{
			for (auto& itItem : m_renderItems)
			{
				// ready shader program
				itItem->BindToDC(poDC);

				// draw mesh
				itItem->Draw(poDC);

				itItem->UnbindFromDC(poDC);
			};

			// -- clear queue after each full draw
			ClearItems();
		}
	};

	class DxSceneGraph
	{
		// init to as little extend as possible, for the "merge to fix 2 boxes" thing
		DirectX::BoundingBox m_SceneBoundBox = DirectX::BoundingBox({ 0,0,0 }, { 0E-7, 0E-7, 0E-7 });
		DxBaseNode::SharedPtr m_rootNode = DxBaseNode::Create("RootNode");

	public:
		DxBaseNode* GetRootNode()
		{
			return m_rootNode.get();
		}

		/// <summary>
		/// Get a bounding box that is precisely big enough to contain all the meshes in the in the scene graph
		/// </summary>
		/// <returns></returns>
		DirectX::BoundingBox GetRootBoundBox()
		{
			UpdateBoundBoxRecursive(GetRootNode());

			return m_SceneBoundBox;
		}

		void UpdateNodes(DxBaseNode* pRootNode, float timeElapsed)
		{
			pRootNode->Update(timeElapsed);

			for (auto& itChildNode : pRootNode->GetChildren())
			{
				UpdateNodes(itChildNode.get(), timeElapsed);
			};
		}

		void FillRenderBucket(DxBaseNode* pNode, IRenderBucket* pDestRenderQueue)
		{
			pNode->FlushToRenderBucket(pDestRenderQueue);

			for (auto& itChildNode : pNode->GetChildren())
			{
				FillRenderBucket(itChildNode.get(), pDestRenderQueue);
			};
		}

	private:
		void UpdateBoundBoxRecursive(DxBaseNode* node)
		{
			
			for (auto& itChildeNode : node->GetChildren())
			{
				UpdateBoundBoxRecursive(itChildeNode.get());
			}

			// Model's BoundBox has to contain all the mesh bound boxes:
			DirectX::BoundingBox::CreateMerged(
				m_SceneBoundBox,
				m_SceneBoundBox,
				node->GetNodeBoundingBox());
		}
	};

} // namespace rldx
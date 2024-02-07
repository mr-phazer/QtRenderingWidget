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

				// TODO: re-enable!! 
				//if(defaultShaderProgram) // use supplied shader
				//	defaultShaderProgram->BindToDC(poDC);

				// draw mesh
				itItem->Draw(poDC);
			};

			// -- clear queue after each full draw
			ClearItems();
		}
	};

	class DxSceneGraph
	{
		DxBaseNode::SharedPtr m_rootNode = DxBaseNode::Create("RootNode");

	public:
		DxBaseNode* GetRootNode()
		{
			return m_rootNode.get();
		}

		void UpdateNodes(DxBaseNode* pRootNode, float timeElapsed)
		{
			pRootNode->Update(timeElapsed);
						// TODO: REMOVE: debugging code
			pRootNode->SetName("Node Updated!!");
			// END: REMOVE: debugging code

			for (auto& itChildNode : pRootNode->GetChildren())
			{
				UpdateNodes(itChildNode.get(), timeElapsed);
			};
		}

		static void SumBoundBoxes()
		{
			DirectX::BoundingBox testerBB;
		}

		void FetchNodes(DxBaseNode* pRootNode, IRenderBucket* pRenderQueue)
		{
			pRootNode->FlushToRenderQueue(pRenderQueue);

			for (auto& itChildNode : pRootNode->GetChildren())
			{
				FetchNodes(itChildNode.get(), pRenderQueue);
			};
		}
	};

} // namespace rldx
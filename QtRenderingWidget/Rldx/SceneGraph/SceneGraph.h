#pragma once
//#include "..\Rendering\DxMesh.h"
//#include "..\Rendering\DxShaderProgram.h"
//#include "..\Interfaces\IDrawable.h"
//#include "..\Interfaces\IFlushable.h"
#include "..\Interfaces\IRenderBucket.h"
//#include "..\Interfaces\IUpdateable.h"
//#include "..\Managers\DxDeviceManager.h"
#include "..\SceneGraph\BaseNode\DxBaseNode.h"

namespace Rldx {

	class DxMeshRenderBucket : public IRenderBucket
	{
		std::vector<IRenderQueueItem*> m_renderItems;

	public:		
		virtual void AddItem(IRenderQueueItem* renderItem) override { m_renderItems.push_back(renderItem); };
		virtual void Clear() override { m_renderItems.clear(); };

		virtual void Draw(ID3D11DeviceContext* poDeviceContext) override
		{
			for (auto& itItem : m_renderItems)
			{
				// ready shader program
				itItem->BindToDC(poDeviceContext);

				// draw mesh
				itItem->Draw(poDeviceContext);
			}

			// -- clear queue after each full draw
			Clear();
		}
	};

	class DxSceneGraph
	{
		DxBaseNode::SharedPointer m_rootNode = DxBaseNode::Create("RootNode");

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
			}
		}

		void FetchNodes(DxBaseNode* pRootNode, IRenderBucket* pRenderQueue)
		{
			pRootNode->FlushToRenderQueue(pRenderQueue);

			for (auto& itChildNode : pRootNode->GetChildren())
			{
				FetchNodes(itChildNode.get(), pRenderQueue);
			}
		}
	};

} // namespace Rldx
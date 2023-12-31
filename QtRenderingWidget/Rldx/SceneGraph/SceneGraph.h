#pragma once
#include "..\Rendering\DxMesh.h"
#include "..\Rendering\DxShaderProgram.h"
#include "..\Interfaces\IDrawable.h"
#include "..\Interfaces\IFlushable.h"
#include "..\Interfaces\IRenderQueue.h"
#include "..\Interfaces\IUpdateable.h"
#include "..\Managers\DxDeviceManager.h"
#include "..\SceneGraph\BaseNode\DxBaseNode.h"

namespace Rldx {

	class DxStandardRenderQueue : public IDxRenderQueue
	{
		std::vector<DxRenderQueueItem> m_queueItems;

	public:
		virtual void AddItem(const DxRenderQueueItem& item) override { m_queueItems.push_back(item); };
		virtual void Clear() override { m_queueItems.clear(); };

		virtual void Draw(ID3D11DeviceContext* poDeviceContext) override
		{
			for (auto& itItem : m_queueItems)
			{
				// ready shader program
				itItem.shaderProgram->GetReady(poDeviceContext);

				// draw mesh
				itItem.meshData->Draw(poDeviceContext);
			}

			// -- clear queue after each full draw
			Clear();
		}
	};

	class DxSceneGraph
	{
		DxBaseNode::SharedPointerBase m_rootNode = DxBaseNode::Create("RootNode");

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

		void FetchNodes(DxBaseNode* pRootNode, IDxRenderQueue* pRenderQueue)
		{
			pRootNode->FlushToRenderQueue(pRenderQueue);

			for (auto& itChildNode : pRootNode->GetChildren())
			{
				FetchNodes(itChildNode.get(), pRenderQueue);
			}
		}
	};

} // namespace Rldx
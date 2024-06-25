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

	class DxSceneGraph
	{
		// init to as little extend as possible, for the "merge to fix 2 boxes" thing
		DirectX::BoundingBox m_SceneBoundBox = DirectX::BoundingBox({ -1,-1,-1 }, { 1,1,1 });
		DxBaseNode::SharedPtr m_vmdRootNode = DxBaseNode::Create(L"RootNode");

		std::vector<DxBaseNode*> nodeLookUptable;

	public:
		DxBaseNode* GetRootNode() const;

		/// <summary>
		/// Get a bounding box that is precisely big enough to contain all the meshes in the in the scene graph
		/// </summary>
		/// <returns></returns>
		DirectX::BoundingBox GetRootBoundBox();

		void UpdateNodes(float timeElapsed);
		static void UpdateNodes(DxBaseNode* pNode, float timeElapsed);

		/// <summary>
		/// Fetches all "
		/// </summary>
		/// <param name="pDestRenderBucket"></param>
		void FillRenderBucket(IRenderBucket* pDestRenderBucket);

		void AddNodeToLinearIndexTable(DxBaseNode* node);

	private:
		void FillRenderBucketRecursive(DxBaseNode* pNode, IRenderBucket* pDestRenderQueue);
		static void UpdateNodesRecursive(DxBaseNode* pRootNode, float timeElapsed);
		void UpdateBoundBoxRecursive(DxBaseNode* node);
	};

} // namespace rldx
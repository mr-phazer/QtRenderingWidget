#include <Rldx\Animation\Helpers\PathHelper.h>
#include <Rldx\Rendering\DxMeshRenderBucket.h>
#include <Rldx\SceneGraph\SceneNodes\DxAnimatorServiceNode.h>
#include "DxDeformerNode.h"
#include "DxModelNode.h"

namespace rldx
{
	class IRenderBucket;

	// TODO: Refactor: Animator/Deformer should be a member, not the base class

	/// <summary>
	/// Container for generated "variant mesh"
	/// Contains a series of meshes represent the variant + a deformer(skeleton) that animates them
	/// </summary>
	class DxVariantMeshNode : public DxDeformerNode
	{
		std::string m_skeletonName;
		std::vector<std::shared_ptr<DxMeshData>> m_meshQueue;

	public:
		DxVariantMeshNode() = default;
		DxVariantMeshNode(const std::string& skeletonName)
			:
			m_skeletonName(skeletonName)
		{};

		void Init()
		{
			LoadBindPose(path_helper::GetPathFromSkeletonName(m_skeletonName));

			LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_01.anim)");
			LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_02.anim)");
			LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_03.anim)");
			LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_04.anim)");
			LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_05.anim)");
		}


		void AddModel(DxModelNode* poModelNodex)
		{
			if (poModelNodex->m_activeLod >= poModelNodex->m_lods.size())
				return;

			SetBoundingBox(poModelNodex->GetNodeBoundingBox());

			// Add all mesh data to the queue
			for (auto& itMeshNode : poModelNodex->m_lods[poModelNodex->m_activeLod])
			{
				auto poMeshdata = std::make_shared<DxMeshData>();

				*poMeshdata = itMeshNode->MeshData();

				poMeshdata->poDeformerSourceNode = this;

				m_meshQueue.push_back(poMeshdata);
			};
		}





		void FlushToRenderBucket(IRenderBucket* pRenderQueue) override;

		//std::shared_ptr<DxDeformerNode> CopyNode()
		//{
		//	auto deformerNode = std::make_shared<DxDeformerNode>();


		//	// ReAssign the deformer to all mesh data
		//	for (auto& itMeshData : m_meshQueue)
		//	{
		//		itMeshData->poDeformerSourceNode = m_deformerNode.get();
		//	}
		//}

	};
} // namespace rldx

//#include <Rldx\Animation\Helpers\PathHelper.h>
//#include <Rldx\Rendering\DxMeshRenderBucket.h>
//#include <Rldx\SceneGraph\SceneNodes\DxAnimatorServiceNode.h>
//#include "DxDeformerNode.h"
//#include "DxModelNode.h"
//
//namespace rldx
//{
//	class IRenderBucket;
//
//	// TODO: Refactor: Animator/Deformer should be a member, not the base class
//
//	/// <summary>
//	/// Container for generated "variant mesh"
//	/// Contains a series of meshes represent the variant + a deformer(skeleton) that animates them
//	/// </summary>
//	class DxVariantMeshNode : public DxMeshNode
//	{
//		shared_ptr<DxDeformerNode> m_deformerNode = std::make_shared<DxDeformerNode>();
//		std::string m_skeletonName = "";
//		std::vector<DxMeshNode*> m_meshQueue;
//
//	public:
//		DxVariantMeshNode() = default;
//		DxVariantMeshNode(const std::string& skeletonName)
//			:
//			m_skeletonName(skeletonName)
//		{};
//
//		void Init()
//		{
//			return;
//			m_deformerNode->LoadBindPose(path_helper::GetPathFromSkeletonName(m_skeletonName));
//
//			m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_01.anim)");
//			m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_02.anim)");
//			m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_03.anim)");
//			m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_04.anim)");
//			m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_05.anim)");
//
//			this->AddChild(m_deformerNode);
//		}
//
//
//		void AddModel(DxModelNode* poModelNodex)
//		{
//			/*if (poModelNodex->m_activeLod >= poModelNodex->m_lods.size())
//				return;*/
//
//			GetNodeBoundingBox() = poModelNodex->GetNodeBoundingBox();
//
//			if (poModelNodex->m_lods.empty())
//				return;
//
//			// Add all mesh data to the queue
//			for (auto& itMeshNode : poModelNodex->m_lods[0])
//			{
//				auto& poMeshdata = itMeshNode;
//
//				//				poMeshdata->SetDeformerNode(m_deformerNode.get(), -1);
//
//				m_meshQueue.push_back(poMeshdata.get());
//			};
//		}
//
//
//		/// <summary>
//		/// Override usual behaviour
//		/// Instead flush the mesh list state to the render buacket
//		/// </summary>
//		/// <param name="pRenderQueue"></param>
//		void FlushToRenderBucket(IRenderBucket* pRenderQueue)
//		{
//			UpdateGlobalTransform(0);
//			auto worldMatrix = GetCurrentGlobalTransForm().Transpose();
//
//			for (auto& itMeshNode : m_meshQueue)
//			{
//				itMeshNode->SetNodeWorldTransForm(worldMatrix);
//				itMeshNode->SetMeshWorldTransForm(sm::Matrix::Identity);
//				pRenderQueue->AddItem(&itMeshNode->MeshData());
//			};
//		}
//
//		//std::shared_ptr<DxDeformerNode> CopyNode()
//		//{
//		//	auto deformerNode = std::make_shared<DxDeformerNode>();
//
//
//		//	// ReAssign the deformer to all mesh data
//		//	for (auto& itMeshData : m_meshQueue)
//		//	{
//		//		itMeshData->poDeformerSourceNode = m_deformerNode.get();
//		//	}
//		//}
//
//	};
//} // namespace rldx
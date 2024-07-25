#include <Rldx\Rendering\DxMeshRenderBucket.h>
#include "..\..\Animation\Helpers\SkeletonHelpers.h"
#include "DxDeformerNode.h"
#include "DxModelNode.h"

namespace rldx
{
	class IDeformer
	{
	public:
		virtual void GetFramePoseMatric(const std::vector<sm::Matrix>& dest) = 0;
	};

	class IRenderBucket;

	// TODO: Refactor: Animator/Deformer should be a member, not the base class

	/// <summary>
	/// Container for generated "variant mesh"
	/// Contains a series of models representing the "variant" + a deformer(skeleton) that animates them
	/// </summary>
	class DxVariantMeshNode : public DxModelNode
	{
		std::wstring m_skeletonName;
		std::vector<DxModelNode*> m_models;
		DxDeformerNode* m_poDeformerNode = nullptr;

	public:
		DxVariantMeshNode()
		{
			SetType(SceneNodeTypeEnum::VariantMeshNode);
			SetTypeString(L"Node:DxVariantMeshNode");
			SetName(L"Unnamed");
		}

		virtual ~DxVariantMeshNode() {
			// TODO: REMOVE
			auto DEBUG_BREAK = 1;
		}

		DxVariantMeshNode(rldx::DxResourceManager& resourceManager, const std::wstring& nodeName, DxDeformerNode* poDeformerNode)
			:
			m_poDeformerNode(poDeformerNode)
		{
			DxVariantMeshNode::DxVariantMeshNode();
			SetName(nodeName);

			if (m_poDeformerNode)
			{
				InitAnimation(resourceManager);
			}
		};

		void SetModels(const std::vector<DxModelNode*>& modelsIn)
		{
			m_models = modelsIn;
		}

		void FlushToRenderBucket(IRenderBucket* pRenderQueue) override;

	private:
		void InitAnimation(rldx::DxResourceManager& resourceManager)
		{
			// loads the bind pose frame as animation, so the asset just stands still
			m_poDeformerNode->LoadAnimClip(resourceManager, skel_anim::GetPackPathFromSkeletonName(m_poDeformerNode->GetSkeleton().GetName()));
		}
	};
} // namespace rldx
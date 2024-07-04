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
		DxVariantMeshNode() : DxModelNode(L"Unnamed DxVariantMeshNode") {}

		DxVariantMeshNode(const std::wstring& nodeName, DxDeformerNode* poDeformerNode)
			:
			m_poDeformerNode(poDeformerNode)
		{
			SetName(nodeName);
			InitAnimation();
		};

		void SetModels(const std::vector<DxModelNode*>& modelsIn)
		{
			m_models = modelsIn;
		}

		void FlushToRenderBucket(IRenderBucket* pRenderQueue) override;

	private:
		void InitAnimation()
		{
			// load the bind pose frame as animation, so the asset just stands still
			m_poDeformerNode->LoadAnimClip(skel_anim::GetPackPathFromSkeletonName(m_poDeformerNode->GetSkeleton().GetName()));
		}
	};
} // namespace rldx
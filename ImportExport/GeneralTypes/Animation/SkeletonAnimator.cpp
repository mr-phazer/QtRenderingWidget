#include "SkeletonAnimator.h"

SkeletonFrame SkeletonAnimator::BlendFrames(const SkeletonFrame& frame0, const SkeletonFrame& frame1, float blendFactor)
{
	SkeletonFrame resultFrame;

	auto itFrame1 = frame1.bonesNodes.begin();

	for (size_t iBoneNode = 0; iBoneNode < frame0.bonesNodes.size(); iBoneNode++)
	{
		auto& node_tranform0 = frame0.bonesNodes[iBoneNode].transform;
		auto& node_tranform1 = frame1.bonesNodes[iBoneNode].transform;

		SkeletonBoneNode node;
		node.transform.position = sm::Vector3::Lerp(node_tranform0.position, node_tranform1.position, blendFactor);
		node.transform.rotation = sm::Quaternion::Slerp(node_tranform0.rotation, node_tranform1.rotation, blendFactor);
		node.transform.scale = sm::Vector3::Lerp(node_tranform0.scale, node_tranform1.scale, blendFactor);

		resultFrame.bonesNodes.push_back(node);
	}

	return resultFrame;
}

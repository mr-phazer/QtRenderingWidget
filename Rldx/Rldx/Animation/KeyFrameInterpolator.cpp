#include "KeyFrameInterpolator.h"

namespace skel_anim
{
	SkeletonKeyFrame KeyFrameInterpolator::InterpolateLocalFrames(const SkeletonKeyFrame& frame0, const SkeletonKeyFrame& frame1, float interpolationFactor)
	{
		using namespace DirectX::SimpleMath;

		SkeletonKeyFrame resultFrame;

		for (size_t iBoneNode = 0; iBoneNode < frame0.boneKeys.size(); iBoneNode++)
		{
			auto& node_tranform0 = frame0.boneKeys[iBoneNode];
			auto& node_tranform1 = frame1.boneKeys[iBoneNode];

			SimpleBoneKey node;
			node.translation = Vector3::Lerp(node_tranform0.translation, node_tranform1.translation, interpolationFactor);
			node.rotation = Quaternion::Slerp(node_tranform0.rotation, node_tranform1.rotation, interpolationFactor);
			node.scale = Vector3::Lerp(node_tranform0.scale, node_tranform1.scale, interpolationFactor);

			resultFrame.boneKeys.push_back(node);
		}

		return resultFrame;
	}

}


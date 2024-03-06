#include "SkeletonAnimator.h"

using namespace skel_anim;

void FrameGenerator::GeneateFramePoseMatrices(const SkeletonFrame& frameLocal, std::vector<sm::Matrix> destPoseMatrices)
{
	destPoseMatrices.resize(m_skeleton.bones.size());
	for (size_t iBone = 0; iBone < m_skeleton.bones.size(); iBone++)
	{
		auto& currentBone = m_skeleton.bones[iBone];

		auto& keyGlobal = m_globalKeys.boneKeys[iBone];
		auto& keyLocal = frameLocal.boneKeys[iBone];

		if (currentBone.parentIndex == -1)
		{
			keyGlobal.translation = frameLocal.boneKeys[iBone].translation;
			keyGlobal.rotation = frameLocal.boneKeys[iBone].rotation;
			continue;
		}

		auto& parentGlobal = m_globalKeys.boneKeys[currentBone.parentIndex];

		keyGlobal.rotation = keyLocal.rotation * parentGlobal.rotation;

		keyGlobal.translation = parentGlobal.rotation * sm::Vector3::Transform(keyLocal.translation, parentGlobal.rotation);

		// calculate global transform matrix
		auto translationMatrix = sm::Matrix::CreateTranslation(keyGlobal.translation);
		auto rotationMatrix = sm::Matrix::CreateFromQuaternion(keyGlobal.rotation);
		// add matrix to pose
		destPoseMatrices[iBone] = (rotationMatrix * translationMatrix);
	};
}

SkeletonFrame FrameInterPolator::InterpolateLocalFrames(const SkeletonFrame& frame0, const SkeletonFrame& frame1, float interpolationFactor)
{
	using namespace DirectX::SimpleMath;

	SkeletonFrame resultFrame;

	for (size_t iBoneNode = 0; iBoneNode < frame0.boneKeys.size(); iBoneNode++)
	{
		auto& node_tranform0 = frame0.boneKeys[iBoneNode];
		auto& node_tranform1 = frame1.boneKeys[iBoneNode];

		BoneKey node;
		node.translation = Vector3::Lerp(node_tranform0.translation, node_tranform1.translation, interpolationFactor);
		node.rotation = Quaternion::Slerp(node_tranform0.rotation, node_tranform1.rotation, interpolationFactor);
		node.scale = Vector3::Lerp(node_tranform0.scale, node_tranform1.scale, interpolationFactor);

		resultFrame.boneKeys.push_back(node);
	}

	return resultFrame;
}

#include "AnimationPlayer.h"

namespace skel_anim
{
	void FrameGenerator::GeneateFramePoseMatrices(const SkeletonKeyFrame& frameLocal, std::vector<sm::Matrix> destPoseMatrices)
	{
		SkeletonKeyFrame globalKeys(m_skeleton.boneTable.size());
		destPoseMatrices.resize(m_skeleton.boneTable.size());

		for (size_t iBone = 0; iBone < m_skeleton.boneTable.size(); iBone++)
		{
			auto& currentBone = m_skeleton.boneTable[iBone];

			auto& keyGlobal = globalKeys.boneKeys[iBone];
			auto& keyLocal = frameLocal.boneKeys[iBone];

			if (currentBone.parentIndex == -1)
			{
				keyGlobal.translation = frameLocal.boneKeys[iBone].translation;
				keyGlobal.rotation = frameLocal.boneKeys[iBone].rotation;
				continue;
			}

			auto& parentGlobal = globalKeys.boneKeys[currentBone.parentIndex];

			keyGlobal.rotation = keyLocal.rotation * parentGlobal.rotation;

			keyGlobal.translation =
				parentGlobal.translation +
				sm::Vector3::Transform(keyLocal.translation, parentGlobal.rotation);

			// TODO: test that it is identical
			auto debug_GlobalMatrix = keyGlobal.GetTransForm();
			// calculate global transform matrix
			auto translationMatrix = sm::Matrix::CreateTranslation(keyGlobal.translation);
			auto rotationMatrix = sm::Matrix::CreateFromQuaternion(keyGlobal.rotation);
			auto globalTransform = rotationMatrix * translationMatrix;

			// add matrix to pose
			destPoseMatrices[iBone] = globalTransform;
		};
	}

	SkeletonKeyFrame FrameInterPolator::InterpolateLocalFrames(const SkeletonKeyFrame& frame0, const SkeletonKeyFrame& frame1, float interpolationFactor)
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
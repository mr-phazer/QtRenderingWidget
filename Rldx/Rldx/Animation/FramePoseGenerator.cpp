#include "DataTypes\Skeleton.h"
#include "DataTypes\SkeletonAnimation.h"
#include "FramePoseGenerator.h"

namespace skel_anim
{
	void FramePoseGenerator::GenerateMatrices(const SkeletonKeyFrame& frameLocal, std::vector<sm::Matrix>& destPoseMatrices)
	{
		SkeletonKeyFrame globalKeys(m_skeleton.GetBoneTable().size());
		destPoseMatrices.resize(m_skeleton.GetBoneTable().size());

		for (size_t iBone = 0; iBone < m_skeleton.GetBoneTable().size(); iBone++)
		{
			auto& currentBone = m_skeleton.GetBoneTable()[iBone];

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
			// calculate global boneTransform matrix
			auto translationMatrix = sm::Matrix::CreateTranslation(keyGlobal.translation);
			auto rotationMatrix = sm::Matrix::CreateFromQuaternion(keyGlobal.rotation);
			auto globalTransform = rotationMatrix * translationMatrix;

			// add matrix to pose
			destPoseMatrices[iBone] = globalTransform;
		};
	}

	void FramePoseGenerator::GenerateMatrices(const Skeleton& skeleton, const SkeletonKeyFrame& frameLocal, std::vector<sm::Matrix>& destPoseMatrices)
	{
		SkeletonKeyFrame globalKeys(skeleton.GetBoneTable().size());
		destPoseMatrices.resize(skeleton.GetBoneTable().size());

		for (size_t iBone = 0; iBone < skeleton.GetBoneTable().size(); iBone++)
		{
			auto& currentBone = skeleton.GetBoneTable()[iBone];

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
			// calculate global boneTransform matrix
			auto translationMatrix = sm::Matrix::CreateTranslation(keyGlobal.translation);
			auto rotationMatrix = sm::Matrix::CreateFromQuaternion(keyGlobal.rotation);
			auto globalTransform = rotationMatrix * translationMatrix;

			// add matrix to pose
			destPoseMatrices[iBone] = globalTransform;
		};
	}

	std::vector<sm::Matrix> FramePoseGenerator_PerTrackInterpolation::GenerateFramePoseMatrices(float time)
	{
		SkeletonKeyFrame globalKeys(m_pSkeleton->GetBoneTable().size());
		FramePoseMatrices destPoseMatrices(m_pSkeleton->GetBoneTable().size());

		for (size_t boneIndex = 0; boneIndex < m_pSkeleton->GetBoneTable().size(); boneIndex++)
		{
			auto& keyGlobal = globalKeys.boneKeys[boneIndex];
			auto& currentBone = m_pSkeleton->GetBoneTable()[boneIndex];

			if (currentBone.parentIndex == -1)
			{
				keyGlobal.translation = m_trackReader.GetTranslation(boneIndex, time);
				keyGlobal.rotation = m_trackReader.GetRotation(boneIndex, time);
				continue;
			}

			auto& parentGlobal = globalKeys.boneKeys[currentBone.parentIndex];

			keyGlobal.rotation = m_trackReader.GetRotation(boneIndex, time) * parentGlobal.rotation;
			keyGlobal.translation = parentGlobal.translation + sm::Vector3::Transform(m_trackReader.GetTranslation(boneIndex, time), parentGlobal.rotation);

			destPoseMatrices[boneIndex] = keyGlobal.GetTransForm();
		};

		return destPoseMatrices;
	}

	std::vector<sm::Matrix> FramePoseGenerator_PerFrameInterpolation::GenerateDiscreteFramePoseMatrices(size_t frameIndex)
	{
		SkeletonKeyFrame globalKeys(m_pSkeleton->GetBoneTable().size());
		FramePoseMatrices outPoseMatrices(m_pSkeleton->GetBoneTable().size());

		outPoseMatrices.resize(m_pSkeleton->GetBoneTable().size());

		for (size_t iBone = 0; iBone < m_pSkeleton->GetBoneTable().size(); iBone++)
		{
			auto& currentBone = m_pSkeleton->GetBoneTable()[iBone];

			auto& keyGlobal = globalKeys.boneKeys[iBone];
			auto& keyLocal = m_pAnimClip->frameData.frames[frameIndex].boneKeys[iBone];

			if (currentBone.parentIndex == -1)
			{
				keyGlobal.translation = keyLocal.translation;
				keyGlobal.rotation = keyLocal.rotation;
				continue;
			}

			auto& parentGlobal = globalKeys.boneKeys[currentBone.parentIndex];

			keyGlobal.rotation = keyLocal.rotation * parentGlobal.rotation;
			keyGlobal.translation = parentGlobal.translation + sm::Vector3::Transform(keyLocal.translation, parentGlobal.rotation);

			outPoseMatrices[iBone] = keyGlobal.GetTransForm();
		};

		return outPoseMatrices;
	}
} // namespace skel_anim
#include "FramePoseGenerator.h"


std::vector<sm::Matrix> skel_anim::FramePoseGenerator_PerTrackInterpolation::GenerateFramePoseMatrices(float time)
{
	SkeletonKeyFrame globalKeys(m_pSkeleton->boneTable.size());
	FramePoseMatrices destPoseMatrices(m_pSkeleton->boneTable.size());

	for (size_t boneIndex = 0; boneIndex < m_pSkeleton->boneTable.size(); boneIndex++)
	{
		auto& currentBone = m_pSkeleton->boneTable[boneIndex];
		auto& keyGlobal = globalKeys.boneKeys[boneIndex];

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

std::vector<sm::Matrix> skel_anim::FramePoseGenerator_PerFrameInterpolation::GenerateDiscreteFramePoseMatrices(size_t frameIndex)
{
	SkeletonKeyFrame globalKeys(m_pSkeleton->boneTable.size());
	FramePoseMatrices outPoseMatrices(m_pSkeleton->boneTable.size());

	outPoseMatrices.resize(m_pSkeleton->boneTable.size());

	for (size_t iBone = 0; iBone < m_pSkeleton->boneTable.size(); iBone++)
	{
		auto& currentBone = m_pSkeleton->boneTable[iBone];

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
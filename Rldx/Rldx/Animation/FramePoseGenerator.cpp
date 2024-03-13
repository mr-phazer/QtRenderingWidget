#include "FramePoseGenerator.h"


std::vector<sm::Matrix> skel_anim::FramePoseMatrixGenerator::GenerateFramePoseMatrices(float time)
{
	SkeletonKeyFrame globalKeys(m_pSkeleton->boneTable.size());
	FramePoseMatrices destPoseMatrices(m_pSkeleton->boneTable.size());

	for (size_t boneIndex = 0; boneIndex < m_pSkeleton->boneTable.size(); boneIndex++)
	{
		auto& currentBone = m_pSkeleton->boneTable[boneIndex];

		auto& keyGlobal = globalKeys.boneKeys[boneIndex];

		if (currentBone.parentIndex == -1)
		{
			keyGlobal.translation = m_trackReader.GetTranslation(boneIndex);
			keyGlobal.rotation = m_trackReader.GetRotation(boneIndex);
			continue;
		}

		auto& parentGlobal = globalKeys.boneKeys[currentBone.parentIndex];

		keyGlobal.rotation = m_trackReader.GetRotation(boneIndex) * parentGlobal.rotation;

		keyGlobal.translation =
			parentGlobal.translation +
			sm::Vector3::Transform(m_trackReader.GetTranslation(boneIndex), parentGlobal.rotation);

		// calculate global transform matrix
		auto translationMatrix = sm::Matrix::CreateTranslation(keyGlobal.translation);
		auto rotationMatrix = sm::Matrix::CreateFromQuaternion(keyGlobal.rotation);

		// add matrix to pose vector
		destPoseMatrices[boneIndex] = (rotationMatrix * translationMatrix);
	};

	return destPoseMatrices;
}
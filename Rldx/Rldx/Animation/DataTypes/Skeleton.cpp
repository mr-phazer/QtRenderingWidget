#include "Skeleton.h"

namespace skel_anim
{
	Skeleton& Skeleton::operator=(Skeleton& inputFile)
	{
		boneTable = inputFile.boneTable;
		inverseBindPoseMatrices = inputFile.inverseBindPoseMatrices;
		bindposeMatrices = inputFile.bindposeMatrices;
		bindPose = inputFile.bindPose;

		return *this;
	}

	Skeleton::Skeleton(const anim_file::AnimFile& inputFile)
	{
		SetBoneTable(inputFile);
	}

	void Skeleton::SetBoneTable(const anim_file::AnimFile& inputFile)
	{
		boneTable.clear();
		for (const auto& itBone : inputFile.boneTable.bones)
		{
			SkeletonBoneNode node;

			node.name = itBone.strName;
			node.boneIndex = itBone.id;
			node.parentIndex = itBone.parent_id;

			boneTable.push_back(node);
		}

		for (int32_t iChildBone = 0; iChildBone < boneTable.size(); iChildBone++)
		{
			auto& parentBone = boneTable[iChildBone].parentIndex;

			if (parentBone != NO_PARENT)
			{
				if (parentBone >= boneTable.size())
				{
					throw std::exception("Parent index out of bounds");
				}

				boneTable[parentBone].children.push_back(&boneTable[iChildBone]);
			}
		}
	}

	int32_t Skeleton::GetIndexFromBoneName(const std::string& boneName) const
	{
		for (int32_t boneIndex = 0; boneIndex < boneTable.size(); boneIndex++)
		{
			if (CompareNoCase(boneTable[boneIndex].name, boneName))
			{
				return boneIndex;
			}
		}

		return -1;
	}
}

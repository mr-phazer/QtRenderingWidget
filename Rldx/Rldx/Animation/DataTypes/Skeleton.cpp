#include <CustomExceptions\CustomExceptions.h>
#include <Rldx\Animation\AnimationPlayer.h>
#include "Skeleton.h"

namespace skel_anim
{
	using namespace utils;

	Skeleton::Skeleton(const anim_file::AnimFile& inputFile)
	{
		auto animation = SkeletonAnimation::CreateFromAnimFile(inputFile);

		SetBoneTable(inputFile);
		FramePoseGenerator(*this).GenerateMatrices(animation->frameData.frames[0], m_bindposeMatrices);

		for (auto& m : m_bindposeMatrices) {
			m_inverseBindPoseMatrices.push_back(m.Invert());
		}
	}

	/*Skeleton& Skeleton::operator=(const Skeleton& inputFile)
	{
		boneTable = inputFile.boneTable;
		m_inverseBindPoseMatrices = inputFile.m_inverseBindPoseMatrices;
		m_bindposeMatrices = inputFile.m_bindposeMatrices;

		return *this;
	}*/

	const std::vector<SkeletonBoneNode>& Skeleton::GetBoneTable() const
	{
		return boneTable;
	}

	void Skeleton::SetBoneTable(const anim_file::AnimFile& inputFile)
	{
		FillBoneTable(inputFile);

		AddChildren();
	}

	void Skeleton::AddChildren()
	{
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

	void Skeleton::FillBoneTable(const anim_file::AnimFile& inputFile)
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
	}

	int32_t Skeleton::GetIndexFromBoneName(const std::string& boneName) const
	{
		for (int32_t boneIndex = 0; boneIndex < boneTable.size(); boneIndex++)
		{
			if (CompareNoCase(ToWString(boneTable[boneIndex].name), ToWString(boneName)))
			{
				return boneIndex;
			}
		}

		return -1;
	}

	const FramePoseMatrices& Skeleton::GetInverseBindPoseMatrices() const
	{
		return m_inverseBindPoseMatrices;
	}

	const FramePoseMatrices& Skeleton::GetBindPoseMatrices() const
	{
		return m_bindposeMatrices;
	}
}

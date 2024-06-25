#include <CustomExceptions\CustomExceptions.h>
#include <Rldx\Animation\AnimationPlayer.h>
#include <Rldx\Managers\ResourceManager\DxResourceManager.h>
#include "Skeleton.h"

namespace skel_anim
{
	using namespace utils;
	
	Skeleton* Skeleton::Create(const anim_file::AnimFile& inputFile)
	{
		auto skeletonName = ToWString(inputFile.fileHeader.skeletonName);

		// check if skeleton already exists
		auto pSkeleton = rldx::DxResourceManager::Instance()->GetResourceByString<Skeleton>(skeletonName);
		if (pSkeleton) { return pSkeleton; }

		// allocate memmory for skeeton
		pSkeleton = rldx::DxResourceManager::Instance()->AllocEmpty<Skeleton>(skeletonName).GetPtr();

		// create skeleleton from ANIM file
		auto m_animation = SkeletonAnimation::CreateFromAnimFile(inputFile);
		pSkeleton->SetBoneTable(inputFile);

		FramePoseGenerator(*pSkeleton).GenerateMatrices(m_animation->frameData.frames[0], pSkeleton->m_bindposeMatrices);

		for (auto& m : pSkeleton->m_bindposeMatrices) {
			pSkeleton->m_inverseBindPoseMatrices.push_back(m.Invert());
		}

		pSkeleton->m_skeletonName = skeletonName;

		return pSkeleton;
	}


	Skeleton::Skeleton(const anim_file::AnimFile& inputFile)
	{
		// set skeleton name
		m_skeletonName = ToWString(inputFile.fileHeader.skeletonName);

		auto m_animation = SkeletonAnimation::CreateFromAnimFile(inputFile);

		if (m_animation->frameData.frames.empty()) {
			throw ConLogExceptionVerbose(L"No frames i bind pose anim file");
		}

		SetBoneTable(inputFile);
		FramePoseGenerator(*this).GenerateMatrices(m_animation->frameData.frames[0], m_bindposeMatrices);

		for (auto& m : m_bindposeMatrices) {
			m_inverseBindPoseMatrices.push_back(m.Invert());
		}
	}

	std::wstring Skeleton::GetName() const
	{
		return m_skeletonName;
	}

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

			if (parentBone != SkeletonBoneNode::NO_PARENT)
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

	std::wstring Skeleton::GetTypeString() const
	{
		return L"Skeleton";
	}

	rldx::ResourceTypeEnum Skeleton::GetType() const
	{
		return rldx::ResourceTypeEnum::Skeleton;
	}
}

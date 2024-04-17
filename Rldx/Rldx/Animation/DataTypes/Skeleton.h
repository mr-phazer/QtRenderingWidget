#pragma once

#include <FileFormats\Anim\Types\Common\TwAnimFile.h>
#include <SimpleMath.h>
#include "..\..\Tools\tools.h"
#include "SkeletonKeyFrame.h"

namespace skel_anim
{
	using FramePoseMatrices = std::vector<sm::Matrix>;

	/// <summary>
	/// Used to define the parent-child hierachy of a skeleton
	/// </summary>
	struct SkeletonBoneNode
	{
		std::string name = "";
		int32_t boneIndex = -1;
		int32_t parentIndex = -1;

		std::vector<SkeletonBoneNode*> children;
	};

	/// <summary>
	/// Skeleton struct
	/// Used for skeleton animation
	/// Only the minimum is stored here, for flexibility
	/// </summary>
	class Skeleton
	{
		static const int32_t NO_PARENT = -1; // the ANIM file format uses -1 to indicate no parent

		FramePoseMatrices m_inverseBindPoseMatrices; // used to transform vertices from model space to bone space
		FramePoseMatrices m_bindposeMatrices; // used to make the bind pose of the skeleton "stick figure", and others

		std::vector<SkeletonBoneNode> boneTable; // linear array of bones, used in the animation system

	public:
		Skeleton() = default;
		Skeleton(const anim_file::AnimFile& inputFile); // constructor, calls SetBoneTable

		//Skeleton& operator=(const Skeleton& copyFrom); // copy constructor		
		const std::vector<SkeletonBoneNode>& GetBoneTable() const; // used to get the boneTable

		int32_t GetIndexFromBoneName(const std::string& boneName) const; // used to get the index of a bone from its name

		const FramePoseMatrices& GetInverseBindPoseMatrices() const; // used to get the inverse bind pose matrices
		const FramePoseMatrices& GetBindPoseMatrices() const; // used to get the inverse bind pose matrices

	private:
		void SetBoneTable(const anim_file::AnimFile& inputFile); // used to fill the boneTable field
		void AddChildren(); // used to fill the "children" field of of each bone
		void FillBoneTable(const anim_file::AnimFile& inputFile); // used to fill the boneTable field
	};

} // namespace skel_anim
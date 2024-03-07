#pragma once

#include "..\..\FileFormats\Anim\Types\Common\TwAnimFile.h"
#include "Skeleton.h"

namespace skel_anim
{
	class SkeletonCreator
	{
		Skeleton m_skeleton;

	public:
		void Create(const anim_file::TwAnimFile& inputFile);

	private:
		void SetNodeMetaData(anim_file::BoneTable& boneData)
		{
			m_skeleton.bones.resize(boneData.bones.size());

			for (auto& itBone : boneData.bones)
			{
				SkeletonBoneNode bone;
				bone.boneIndex = itBone.id;
				bone.parentIndex = itBone.parent_id;
			}
		}
	};


	class FrameInterPolator
	{
	public:
		static SkeletonFrame InterpolateLocalFrames(const SkeletonFrame& frame0, const SkeletonFrame& frame1, float blendFactor);
	};


	class FrameGenerator
	{
		Skeleton m_skeleton;
		SkeletonFrame m_globalKeys;

	public:
		void GeneateFramePoseMatrices(const SkeletonFrame& frameLocal, std::vector<sm::Matrix> destPoseMatrices);



	};

}

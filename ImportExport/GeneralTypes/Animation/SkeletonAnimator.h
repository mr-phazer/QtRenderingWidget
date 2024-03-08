#pragma once

#include "..\..\FileFormats\Anim\Types\Common\TwAnimFile.h"
#include "Skeleton.h"

namespace skel_anim
{
	class SkeletonCreator
	{
		Skeleton m_skeleton;

	public:
		void Create(const anim_file::TwAnimFile& inputFile)
		{
			SetBoneTable(inputFile);

			size_t bone = 0;
			size_t frame = 0;
			//m_skeleton.bindPose.boneKeys[bone].rotation = inputFile.frames[frame].rotations[bone]
		}

	private:
		void SetBoneTable(const anim_file::TwAnimFile& inputFile)
		{
			m_skeleton.boneTable.clear();
			for (const auto& itBone : inputFile.boneTable.bones)
			{
				SkeletonBoneNode node;
				node.boneIndex = itBone.id;
				node.parentIndex = itBone.parent_id;

				m_skeleton.boneTable.push_back(node);
			}
		}

	private:

	};


	class FrameInterPolator
	{
	public:
		static SkeletonKeyFrame InterpolateLocalFrames(const SkeletonKeyFrame& frame0, const SkeletonKeyFrame& frame1, float blendFactor);
	};


	class FrameGenerator
	{
		Skeleton m_skeleton;

	public:
		FrameGenerator(Skeleton& skeleton) : m_skeleton(skeleton) {}
		void GeneateFramePoseMatrices(const SkeletonKeyFrame& frameLocal, std::vector<sm::Matrix> destPoseMatrices);
	};

}

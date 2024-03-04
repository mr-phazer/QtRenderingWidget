#pragma once

#include "Skeleton.h"
#include "..\..\FileFormats\Anim\DataTypes\BoneTable.h"

namespace animation
{
	class SkeletonBuilder
	{
		Skeleton m_skeleton;
		 
		void SetMetaData(anim_file::BoneTable& boneData)
		{
			m_skeleton.bones.clear();

			for (auto& itBone : boneData.bones)
			{
				SkeletonBoneNode bone;
				bone.boneIndex = itBone.id;
				bone.parentIndex = itBone.parent_id;								
				m_skeleton.bones.push_back(bone);
			}

		}

	};

	class FrameGenerator
	{
		std::vector<sm::Matrix> m_inverseBindPoseMatrices;
	
	
	public:
		FrameGenerator();


		AnimationFrame



	};



	class FrameSomething
	{
		AnimationFrame m_skeleton;
		std::vector<sm::Matrix> m_inverseBindPoseMatrices;

		AnimationFrame m_currentFrame;
		AnimationFrame m_nextFrame;

	public:
		static AnimationFrame BlendFrames(const AnimationFrame& frame0, const AnimationFrame& frame1, float blendFactor);





	};

}

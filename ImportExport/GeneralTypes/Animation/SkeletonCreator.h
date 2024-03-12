
#pragma once

#include "Skeleton.h"

namespace skel_anim {


	class SkeletonCreator
	{
		Skeleton m_skeleton;

	public:
		Skeleton Create(const anim_file::AnimFile& inputFile)
		{
			SetBoneTable(inputFile);

			return m_skeleton;
		}

	private:


	private:

	};

}
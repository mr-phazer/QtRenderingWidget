#pragma once

#include "DataTypes/SkeletonKeyFrame.h"
#include "SkeletonKeyFrameSampler.h"

namespace skel_anim
{
	class KeyFrameInterpolator
	{
	public:
		static SkeletonKeyFrame InterpolateLocalFrames(const SkeletonKeyFrame& frame0,
													   const SkeletonKeyFrame& frame1,
													   float blendFactor);
	};



} // namespace skel_anim
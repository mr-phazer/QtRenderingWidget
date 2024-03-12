#pragma once

#include "AnimTrackReader.h"

namespace skel_anim {

	class FramePoseMatrixGenerator
	{
		AnimTrackReader m_trackReader;
		const Skeleton* m_pSkeleton = nullptr;

	public:
		FramePoseMatrixGenerator() {};
		//FramePoseMatrixGenerator(const Skeleton& skeleton) : m_pSkeleton(&skeleton) {};

		/// <summary>
		/// Skeleton'bone table is for "hierachy" when generator global transform	
		/// </summary>		
		void SetSkeleton(const Skeleton* pSkeleton) { m_pSkeleton = pSkeleton; };

		// TODO: maybe make into "getTrackReader()"?
		/// <summary>
		/// Set the current animation clip to process
		/// </summary>		
		void SetAnimCLip(const SkeletonAnimationClip* pAnimClip)
		{
			m_trackReader.SetAnimClip(pAnimClip);
		};

		std::vector<sm::Matrix> GenerateFramePoseMatrices(float time = 0.0f);
	};
}
#include "Rldx\Rldx\Animation\DataTypes\SkeletonAnimation.h"
#include <Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h>

#include <ImportExport\FileFormats\Anim\Types\Common\TwAnimFile.h>
#include <CommonLibs\Utils\StrUtils.h>

#include "Skeleton.h"
#include "SkeletonAnimation.h"

namespace skel_anim
{
	using namespace utils;

	SkeletonAnimation* SkeletonAnimation::CreateFromAnimFile(const anim_file::AnimFile& inAnimFile)
	{
		auto newAnim = rldx::DxResourceManager::Instance()->AllocAnim().GetPtr();

		newAnim->m_skeletonName = ToWString(inAnimFile.fileHeader.skeletonName);
		newAnim->lastKeyTime = inAnimFile.fileHeader.fLastKeyTime;
		newAnim->keysPerSecond = inAnimFile.fileHeader.fFrameRate;

		for (const auto& frame : inAnimFile.frames)
		{
			newAnim->frameData.frames.push_back(SkeletonKeyFrame::CreateFromCommonFrame(frame));
			newAnim->frameData.boneBlendWeights.push_back(1.0f);
			newAnim->frameData.boneSpliceMask.push_back(true);
		}

		return newAnim;
	};
} // namespace skel_anim
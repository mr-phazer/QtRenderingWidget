#include "SkeletonAnimation.h"

#include <FileFormats\Anim\Types\Common\TwAnimFile.h>
#include <Rldx\Managers\ResourceManager\DxResourceManager.h>
#include "..\..\Animation\DataTypes\SkeletonAnimation.h"

namespace skel_anim
{
	SkeletonAnimation* SkeletonAnimation::CreateFromAnimFile(const anim_file::AnimFile& in)
	{
		auto newAnim = rldx::DxResourceManager::Instance()->AllocAnim().GetPtr();

		newAnim->lastKeyTime = in.fileHeader.fLastKeyTime;
		newAnim->keysPerSecond = in.fileHeader.fFrameRate;

		for (const auto& frame : in.frames)
		{
			newAnim->frameData.frames.push_back(SkeletonKeyFrame::CreateFromCommonFrame(frame));
			newAnim->frameData.boneBlendWeights.push_back(1.0f);
			newAnim->frameData.boneSpliceMask.push_back(true);
		}

		return newAnim;
	}
	std::wstring SkeletonAnimation::GetTypeString() const
	{
		return L"Animation";
	}

	rldx::ResourceTypeEnum SkeletonAnimation::GetType() const
	{
		return rldx::ResourceTypeEnum::Animation;
	}
}
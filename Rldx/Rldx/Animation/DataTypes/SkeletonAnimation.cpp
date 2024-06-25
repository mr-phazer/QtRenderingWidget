#include <FileFormats\Anim\Types\Common\TwAnimFile.h>
#include <Rldx\Managers\ResourceManager\DxResourceManager.h>
#include <Rldx\Tools\tools.h>
#include "..\..\Animation\DataTypes\SkeletonAnimation.h"
#include "Skeleton.h"
#include "SkeletonAnimation.h"

namespace skel_anim
{
	SkeletonAnimation* SkeletonAnimation::CreateFromAnimFile(const anim_file::AnimFile& inAnimFile)
	{
		auto newAnim = rldx::DxResourceManager::Instance()->AllocAnim().GetPtr();

		newAnim->m_skeletonName = libtools::WidenString(inAnimFile.fileHeader.skeletonName);
		newAnim->lastKeyTime = inAnimFile.fileHeader.fLastKeyTime;
		newAnim->keysPerSecond = inAnimFile.fileHeader.fFrameRate;

		for (const auto& frame : inAnimFile.frames)
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
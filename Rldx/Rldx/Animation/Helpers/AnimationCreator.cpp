#include <FileFormats\Anim\Reader\TwAnimReader.h>
#include <memory>
#include <string>
#include "..\DataTypes\SkeletonAnimation.h"
#include "..\Rldx\Rldx\Animation\DataTypes\Skeleton.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"
#include "AnimationCreator.h"
#include "SkeletonHelpers.h"

namespace skel_anim
{
	// TODO: maybe clean all this up, so the skeleton doesn't has to be loaded every time
	// TODO: although, it will require a lot of refactoring, to still support the VMD-rome_man <-> rome_man_game thing
	void skel_anim::AnimationCreator::CreateAnimationFromFile() {

		// get anim file data bytes
		auto animBytes = rldx::DxResourceManager::GetFile(m_animFilePath);

		// TODO: Static method for "GetSkeletonName"?
		// fetch skeleton m_nodeName from anim file
		auto skeletonName = anim_file::TwAnimFileReader().GetSkeletonName(animBytes);

		// get bind pose file, as TwAnimReader needs it for dequantization
		auto bindPoseBytes = rldx::DxResourceManager::GetFile(skel_anim::GetPackPathFromSkeletonName(skeletonName));

		// fill create bind pose field
		m_spoBindPoseAnimFile = std::make_unique<anim_file::AnimFile>(anim_file::TwAnimFileReader().Read(bindPoseBytes));

		// dequantize m_animation, using bind pose
		m_spoAnimationAnimFile = std::make_unique<anim_file::AnimFile>(anim_file::TwAnimFileReader().Read(animBytes, m_spoBindPoseAnimFile.get()));

		m_animation = SkeletonAnimation::CreateFromAnimFile(*m_resourceManager, *m_spoAnimationAnimFile);
	}

	AnimationCreator::AnimationCreator(rldx::DxResourceManager& resourceManager, const std::wstring& animFilePath, const Skeleton& targetSkeleton)
		:
		m_resourceManager(&resourceManager),
		m_animFilePath(animFilePath),
		m_targetSkeleton(&targetSkeleton)
	{
		CreateAnimationFromFile();
	}
} // namespace skel_anim
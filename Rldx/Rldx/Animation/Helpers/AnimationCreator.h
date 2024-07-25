#pragma once

#include "Utils\StrUtils.h"

namespace skel_anim
{
	struct SkeletonAnimation;
	class Skeleton;

	class AnimationCreator
	{
		std::wstring m_animFilePath;
		const Skeleton* m_targetSkeleton;
		rldx::DxResourceManager* m_resourceManager;

		SkeletonAnimation* m_animation; // ready to use m_animation data
		std::unique_ptr<anim_file::AnimFile> m_spoAnimationAnimFile; // raw m_animation data, no use for it yet
		std::unique_ptr<anim_file::AnimFile> m_spoBindPoseAnimFile; // raw bind pose data, for creating skeletons for animations remapping

	private:
		// TODO: maybe this should not be called in the constructor, maybe the user should call it manually with path/etc params?
		void CreateAnimationFromFile();
	public:
		AnimationCreator(rldx::DxResourceManager& resourceManager, const std::wstring& animFilePath, const Skeleton& targetSkeleton);

		SkeletonAnimation* GetAnimation() const { return m_animation; }
		const anim_file::AnimFile* GetAnimationAnimFile() const { return m_spoAnimationAnimFile.get(); }
		const anim_file::AnimFile* GetBindPoseAnimFile() const { return m_spoAnimationAnimFile.get(); }
		std::wstring GetSkeletonName() const { return utils::ToWString(std::string(m_spoAnimationAnimFile->fileHeader.skeletonName)); }
	};
} // namespace skel_anim
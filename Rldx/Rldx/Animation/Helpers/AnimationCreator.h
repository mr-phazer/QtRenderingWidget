#pragma once

namespace skel_anim
{
	struct SkeletonAnimation;
	class Skeleton;

	class AnimationCreator
	{
		std::wstring m_animFilePath;
		const Skeleton* m_targetSkeleton;

		SkeletonAnimation* m_animation; // ready to use m_animation data
		std::unique_ptr<anim_file::AnimFile> m_spoAnimationAnimFile; // raw m_animation data, no use for it yet
		std::unique_ptr<anim_file::AnimFile> m_spoBindPoseAnimFile; // raw bind pose data, for creating skeletons for animations remapping

	private:
		void CreateAnimationFromFile();
	public:
		AnimationCreator(const std::wstring& m_animFilePath, const Skeleton& m_targetSkeleton);

		SkeletonAnimation* GetAnimation() const { return m_animation; }
		const anim_file::AnimFile* GetAnimationAnimFile() const { return m_spoAnimationAnimFile.get(); }
		const anim_file::AnimFile* GetBindPoseAnimFile() const { return m_spoAnimationAnimFile.get(); }
		std::wstring GetSkeletonName() const { return libtools::string_to_wstring(std::string(m_spoAnimationAnimFile->fileHeader.skeletonName)); }
	};
} // namespace skel_anim
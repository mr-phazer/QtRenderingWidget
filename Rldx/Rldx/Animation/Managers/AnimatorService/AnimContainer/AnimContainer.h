#pragma once
namespace skel_anim
{
	// forward declarations
	struct SkeletonAnimation;
	struct SkeletonKeyFrame;


	/// <summary>
	/// contains 1 animations clip, client can sample interpolated keyframes from it
	/// </summary>
	class AnimContainer
	{
		SkeletonAnimation* m_animation = nullptr;

	public:
		AnimContainer() = default;
		AnimContainer(SkeletonAnimation* m_animation) : m_animation(m_animation) {}

		void SetAnimation(SkeletonAnimation* m_animation);

		SkeletonKeyFrame Sample(float time);

		float GetLenth() const;
	};
} // namespace skel_anim
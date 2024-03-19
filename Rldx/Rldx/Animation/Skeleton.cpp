#include "Skeleton.h"

namespace anim_file { struct AnimFrameCommon; }

namespace skel_anim
{
	sm::Matrix SimpleBoneKey::GetTransForm() const
	{
		// calculate boneTransform matrix
		auto translationMatrix = sm::Matrix::CreateTranslation(translation);
		auto rotationMatrix = sm::Matrix::CreateFromQuaternion(rotation);

		return (rotationMatrix * translationMatrix);
	}

	SkeletonKeyFrame SkeletonKeyFrame::CreateFromCommonFrame(const anim_file::AnimFrameCommon& input)
	{
		SkeletonKeyFrame newFrame;
		newFrame.boneKeys.resize(input.translations.size());
		for (size_t boneIndex = 0; boneIndex < input.translations.size(); boneIndex++)
		{
			newFrame.boneKeys[boneIndex].translation = input.translations[boneIndex];
			newFrame.boneKeys[boneIndex].rotation = input.rotations[boneIndex];
		}

		return newFrame;
	}

	/*SkeletonAnimation skel_anim::SkeletonAnimation::CreateFromTWAnim(const anim_file::TwAnimFile& in)
	{
		SkeletonAnimation out;

		out.lastKeyTime = in.fileHeader.fLastKeyTime;
		out.keysPerSecond = in.fileHeader.fFrameRate;

		out.frames = in.frames;

		return out;
	}*/
}

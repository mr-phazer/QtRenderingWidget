#include "Skeleton.h"

#include "..\..\GeneralTypes\Animation\AnimFrameCommon.h"

namespace skel_anim
{
	sm::Matrix SimpleBoneKey::GetTransForm() const
	{
		// calculate transform matrix
		auto translationMatrix = sm::Matrix::CreateTranslation(translation);
		auto rotationMatrix = sm::Matrix::CreateFromQuaternion(rotation);

		return (rotationMatrix * translationMatrix);
	}

	SkeletonKeyFrame SkeletonKeyFrame::CreateFromCommonFrame(const AnimFrameCommon& input)
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
	/*SkeletonAnimationClip skel_anim::SkeletonAnimationClip::CreateFromTWAnim(const anim_file::TwAnimFile& in)
	{
		SkeletonAnimationClip out;

		out.clipLength = in.fileHeader.fEndTime;
		out.keysPerSecond = in.fileHeader.fFrameRate;

		out.frames = in.frames;

		return out;
	}*/
}

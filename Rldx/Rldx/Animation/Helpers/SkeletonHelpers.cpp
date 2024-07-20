#include "SkeletonHelpers.h"

#include <ImportExport\FileFormats\Anim\Reader\TwAnimReader.h>
#include <CommonLibs\Utils\ByteStream.h>
#include <CommonLibs/Utils/ByteStream.h>

#include <Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h>
#include <string>
#include "..\..\Animation\DataTypes\Skeleton.h"

namespace skel_anim
{
	using namespace utils;

	const std::wstring packSkeletonsPath = L"animations/skeletons/";
	const std::wstring packSkeletonExtention = L".anim";

	std::wstring GetPackPathFromSkeletonName(const std::wstring& skeletonName)
	{
		return	packSkeletonsPath + skeletonName + packSkeletonExtention;
	}

	Skeleton GetAnimFromFile(const std::wstring& skeletonName)
	{
		ByteStream skeletonFileBinary(rldx::DxResourceManager::GetFile(GetPackPathFromSkeletonName(skeletonName)));
		auto animFileSkeleton = anim_file::TwAnimFileReader().Read(skeletonFileBinary);

		return Skeleton(animFileSkeleton);
	}

	/// <summary>
	/// For Rome2/Atilla/ToB, has 2 versions of the same skeleton, we dorrect the skeleton to most detailed version, to support all anims
	/// </summary>
	/// <param m_nodeName="inOutSkeleton"></param>
	void ForceCorrectSkeleton(skel_anim::Skeleton& inOutSkeleton)
	{
		if (CompareNoCase(inOutSkeleton.GetName(), L"rome_man_game"))
		{
			ByteStream animSkeletonBinaryData(LR"(animations/skeletons/rome_man.anim")");
			auto animSkeletonFile = anim_file::TwAnimFileReader().Read(animSkeletonBinaryData);
			inOutSkeleton = skel_anim::Skeleton(animSkeletonFile);
		}
	}

}
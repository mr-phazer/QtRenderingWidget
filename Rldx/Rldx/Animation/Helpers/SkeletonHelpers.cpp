#include "SkeletonHelpers.h"

#include <ImportExport\FileFormats\Anim\Reader\TwAnimReader.h>
#include <ImportExport\Helpers\ByteStream.h>

#include <Rldx\Managers\ResourceManager\DxResourceManager.h>
#include <Rldx\Tools\tools.h>
#include <string>
#include "..\..\Animation\DataTypes\Skeleton.h"

namespace skel_anim
{
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

	void ForceCorrectSkeleton(skel_anim::Skeleton& inOutSkeleton)
	{
		if (CompareNoCase(inOutSkeleton.GetName(), L"rome_man_game"))
		{
			ByteStream animSkeletonBinaryData(LR"(animations/skeletons/rome_man.anim")");
			auto animSkeletonFile = anim_file::TwAnimFileReader().Read(animSkeletonBinaryData);
			inOutSkeleton = skel_anim::Skeleton(animSkeletonFile);
		}
	}
	std::wstring GetPackPathFromSkeletonName(const std::string& skeletonName)
	{
		return std::wstring();
	}
}
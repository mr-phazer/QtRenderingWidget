#pragma once

#include "Utils\ByteStream.h"
#include "..\Types\Common\BoneTable.h"

namespace anim_file
{
	class BoneTableCreator
	{
	public:
		BoneTable Create(ByteStream& bytes, size_t boneCount)
		{
			BoneTable outBoneTable;
			outBoneTable.bones.resize(boneCount);
			uint32_t boneIndex = 0;
			for (auto& itBone : outBoneTable.bones)
			{
				itBone.id = boneIndex++;
				itBone.strName = bytes.ReadLengthPrefixed16StringA();
				itBone.parent_id = bytes.TReadElement<int32_t>();
			}

			return outBoneTable;
		}
	};
};




#pragma once

#include "..\..\..\Helpers\ByteStream.h"
#include "..\DataTypes\BoneTable.h"

namespace anim_file
{
	class BoneTableCreator
	{	
		BoneTable & outBoneTable;
	public:
		BoneTableCreator() = delete;
		BoneTableCreator(BoneTable& destBoneTable) : outBoneTable(destBoneTable) { }
		void Create(ByteStream& bytes, size_t boneCount)
		{					
			outBoneTable.bones.resize(boneCount);
			uint32_t boneIndex = 0;
			for (auto& itBone : outBoneTable.bones)
			{
				itBone.id = boneIndex++;
				itBone.strName = bytes.ReadLengthPrefixed16StringA();
				itBone.parent_id = bytes.TReadElement<int32_t>();
			}			
		}
	};
};




#pragma once

#pragma once

#include "..\..\..\Helpers\ByteStream.h"
#include "..\..\DataTypes\MaterialBlockCommon.h"
#include "..\..\DataTypes\MeshHeaderType5.h"

namespace rmv2 {

	class IMaterialBlockCommonCreator
	{
	public:
		virtual MaterialBlockCommon Create(ByteStream& bytes, const MeshHeaderType5& header) = 0;
	};

	class MaterialBlockCommonCreator : IMaterialBlockCommonCreator
	{
	public:
		MaterialBlockCommon Create(ByteStream& bytes, const MeshHeaderType5& header) override;
		
		
	private:		
		void ReadExtraMaterialParams(ByteStream& bytes, rmv2::MaterialBlockCommon& matBlock, const rmv2::MeshHeaderType5& header);
		void ReadTextures(ByteStream& bytes, rmv2::MaterialBlockCommon& matBlock, const rmv2::MeshHeaderType5& header);
		void ReadAttachPointTable(ByteStream& bytes, rmv2::MaterialBlockCommon& matBlock, const rmv2::MeshHeaderType5& header);
	};

}
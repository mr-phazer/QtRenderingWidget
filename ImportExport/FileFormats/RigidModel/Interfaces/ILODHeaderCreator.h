#pragma once

#include "..\DataTypes\LodHeader.h"

namespace rmv2 {	
	
	class ILODHeaderCreator
	{
	public:
		virtual LODHeaderCommon Create(ByteStream& bytes) = 0;
		virtual size_t GetHeaderSize() = 0;
	};
}
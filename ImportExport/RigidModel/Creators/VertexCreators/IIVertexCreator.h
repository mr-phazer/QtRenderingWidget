#pragma once

#include "..\..\..\Helpers\ByteStream.h"
#include "..\..\..\..\rldx\DataTypes\ConstBuffers\CPUConstBuffers.h"

namespace rmv2 {

	class IVertexCreator
	{
		virtual rldx::CommonVertex Create(ByteStream& bytes) = 0;
	};
};
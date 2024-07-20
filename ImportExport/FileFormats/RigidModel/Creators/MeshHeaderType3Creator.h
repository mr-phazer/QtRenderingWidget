#pragma once

#include "..\Types\Common\MeshHeaderType3.h"
#include <CommonLibs\Utils\ByteStream.h>

namespace rmv2 {

	class IMeshHeaderType3Creator
	{
	public:
		virtual MeshHeaderType3 Create(utils::ByteStream& bytes) = 0;
	};

	class MeshHeaderType3Creator : public IMeshHeaderType3Creator
	{
	public:
		MeshHeaderType3 Create(utils::ByteStream& bytes) override;
	};

}; // namespace rmv2



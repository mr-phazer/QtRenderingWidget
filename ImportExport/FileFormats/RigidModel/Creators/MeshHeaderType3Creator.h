#pragma once

#include "..\..\..\Helpers\ByteStream.h"
#include "..\Types\Common\MeshHeaderType3.h"

namespace rmv2 {

	class IMeshHeaderType3Creator
	{
	public:
		virtual MeshHeaderType3 Create(ByteStream& bytes) = 0;
	};

	class MeshHeaderType3Creator : public IMeshHeaderType3Creator
	{
	public:
		MeshHeaderType3 Create(ByteStream& bytes) override;
	};

}; // namespace rmv2



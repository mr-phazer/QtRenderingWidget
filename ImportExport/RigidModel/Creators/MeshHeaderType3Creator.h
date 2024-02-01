#pragma once

#include "..\DataTypes\MeshHeaderType3.h"
#include "..\..\Helpers\ByteStream.h"

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


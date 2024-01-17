#pragma once

#include "..\DataTypes\MeshHeaderType5.h"

namespace rmv2 {

	class IMeshHeaderType5Creator
	{
	public:
		virtual MeshHeaderType5 Create(ByteStream& bytes) = 0;
	};

	class MeshHeaderType5Creator : public IMeshHeaderType5Creator
	{
	public:
		MeshHeaderType5 Create(ByteStream& bytes) override;
	};

}; // namespace rmv2



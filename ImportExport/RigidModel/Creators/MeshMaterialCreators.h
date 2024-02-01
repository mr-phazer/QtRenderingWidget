#pragma once

#include "..\DataTypes\MeshHeaderType5.h"
#include "..\..\Helpers\ByteStream.h"
#include "..\..\Helpers\Templates.h"

namespace rmv2 {

	class IMeshMaterialCreator
	{
	public:
		virtual MeshHeader5Common Create(ByteStream& bytes) = 0;
	};

	class MaterielHeaderType5Creator : public IMeshMaterialCreator
	{
	public:
		MeshHeader5Common Create(ByteStream& bytes) override;
	};

	

}; // namespace rmv2



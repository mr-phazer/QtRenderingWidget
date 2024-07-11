#pragma once

#include "Utils\ByteStream.h"
#include "..\..\..\Helpers\Templates.h"
#include "..\Types\Common\MeshHeaderType5.h"

namespace rmv2 {

	class IMeshMaterialCreator
	{
	public:
		virtual MaterialHeaderType5 Create(utils::ByteStream& bytes) = 0;
	};

	class MaterielHeaderType5Creator : public IMeshMaterialCreator
	{
	public:
		MaterialHeaderType5 Create(utils::ByteStream& bytes) override;
	};



}; // namespace rmv2



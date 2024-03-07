#pragma once

#include "..\..\..\Helpers\ByteStream.h"
#include "..\..\..\Helpers\Templates.h"
#include "..\Types\Common\MeshHeaderType5.h"

namespace rmv2 {

	class IMeshMaterialCreator
	{
	public:
		virtual MaterialHeaderType5 Create(ByteStream& bytes) = 0;
	};

	class MaterielHeaderType5Creator : public IMeshMaterialCreator
	{
	public:
		MaterialHeaderType5 Create(ByteStream& bytes) override;
	};



}; // namespace rmv2



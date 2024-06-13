#pragma once

#include "..\Types\Common\FileHeader.h"

#include "Utils\ByteStream.h"
#include "..\..\..\Helpers\Templates.h"



#define ABSTRACT = 0

namespace rmv2 {

	class IFileHeaderCreator : public templates::TCreator<FileHeaderCommon> { };

	class FileHeaderCreatorDefault : public IFileHeaderCreator
	{
	public:
		FileHeaderCommon Create(ByteStream&);
	};
}; // namespace rmv2
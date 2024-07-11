#pragma once

#include "..\Types\Common\FileHeader.h"

#include "..\..\..\Helpers\Templates.h"
#include "Utils\ByteStream.h"

namespace rmv2 {

	class IFileHeaderCreator : public templates::TCreator<FileHeaderCommon> { };

	class FileHeaderCreatorDefault : public IFileHeaderCreator
	{
	public:
		FileHeaderCommon Create(utils::ByteStream&);
	};
}; // namespace rmv2
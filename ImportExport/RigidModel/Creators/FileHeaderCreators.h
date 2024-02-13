#pragma once

#include "..\DataTypes\FileHeader.h"

#include "..\..\Helpers\ByteStream.h"
#include "..\..\Helpers\Templates.h"



#define ABSTRACT = 0

namespace rmv2 {	

		class IFileHeaderCreator : public templates::TByteCreator<FileHeaderCommon> { };

		class FileHeaderCreatorDefault : public IFileHeaderCreator
		{
		public:
			FileHeaderCommon Create(ByteStream&);
		};
}; // namespace rmv2
#pragma once

#include "..\DataTypes\FileHeader.h"

#include "..\..\Helpers\ByteStream.h"
#include "..\..\Helpers\Templates.h"



#define ABSTRACT = 0

namespace rmv2 {
	namespace file_header {		

		class IFileHeaderCreator : public helpers::TByteCreator<FileHeaderCommon> { };

		class FileHeaderCreatorDefault : public IFileHeaderCreator
		{
		public:
			FileHeaderCommon Create(ByteStream&);
		};



	}; // namespace file_header

}; // namespace rmv2
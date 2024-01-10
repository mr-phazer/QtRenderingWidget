#pragma once

#include "..\..\Helpers\ByteStream.h"
#include "..\DataTypes\FileHeader.h"

#define ABSTRACT = 0

namespace rmv2 {
	namespace file_header {		

		class FileHeaderCreatorDefault : public IFileHeaderCreator
		{
		public:
			FileHeaderCommon Create(ByteStream&);
		};



	}; // namespace file_header

}; // namespace rmv2
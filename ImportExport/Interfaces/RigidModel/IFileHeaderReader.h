#pragma once

#include "..\..\Interfaces\IReader.h"

namespace rigidmodel
{
	struct FileHeader
	{
		int version;
	};


	class IFileHeaderReader
	{
	public:
		FileHeader Read(void* pMemory, )
		{
			FileHeader header;
			reader->Read(&header, sizeof(header));
			return header;
		}
	};
}
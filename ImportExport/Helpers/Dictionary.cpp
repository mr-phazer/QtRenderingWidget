#include "Dictionary.h"
#include "..\Interfaces\IReader.h"
#include <memory>


struct CommonLodHeader {};

class ILODReader
{
public:
	virtual void Read(void* buffer, size_t size, CommonLodHeader* poDest) = 0;

};

class LODReaderVersion7 : public ILODReader
{
public:

	virtual void Read(void* buffer, size_t size, CommonLodHeader* poDest)
	{
		auto DEBUG_1 = 1;
	}
};

class LODReaderVersion6 : public ILODReader
{
public:

	virtual void Read(void* buffer, size_t size, CommonLodHeader* poDest)
	{
		auto DEBUG_1 = 1;
	}
};



void DEBUG_TestFunction()
{
enum FileHeaderVesionEnum
{
	V6,
	V7
};

// inside a factory, ofc course
Dictionary<FileHeaderVesionEnum, ILODReader> dict(
	{
		{ FileHeaderVesionEnum::V6,  new LODReaderVersion6},
		{ FileHeaderVesionEnum::V7,  new LODReaderVersion7}

	});

auto readerToUse = dict.Get(V7); // throws if not found
};

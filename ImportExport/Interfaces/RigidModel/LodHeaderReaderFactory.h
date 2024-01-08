#pragma once
#include <map>
#include <memory>

struct LodHeader
{
	int stuff;
};

class ILodHeaderReader
{
	virtual LodHeader Read(void* pSourceMemory, size_t offset) = 0;
};

class LodHeaderReaderV6 : public ILodHeaderReader
{
	virtual LodHeader Read(void* pSourceMemory, size_t offset)
	{
		return LodHeader();
	}
};

class LodHeaderReaderV7 : public ILodHeaderReader
{
	virtual LodHeader Read(void* pSourceMemory, size_t offset)
	{
		return LodHeader();
	}
};

class LodHeaderReaderV8 : public ILodHeaderReader
{
	virtual LodHeader Read(void* pSourceMemory, size_t offset)
	{
		return LodHeader();
	}
};

enum RMV2VersionEnum : int
{
	V5,
	V6,
	V8,
	V7
};

class LodHeaderReaderFactory
{
	std::map<RMV2VersionEnum, std::unique_ptr<ILodHeaderReader>> m_mapReaders;

	LodHeaderReaderFactory()
	{		
		m_mapReaders[RMV2VersionEnum::V6] = std::make_unique<LodHeaderReaderV6>();
		m_mapReaders[RMV2VersionEnum::V7] = std::make_unique<LodHeaderReaderV7>();
		m_mapReaders[RMV2VersionEnum::V8] = std::make_unique<LodHeaderReaderV7>();		
	}

	// ... her goes copy-paste of your code, with minor changes
};


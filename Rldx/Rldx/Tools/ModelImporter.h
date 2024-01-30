#pragma once
#include <memory>
#include <map>


class LodHeaderCommon;
class CommonVertex;
enum Rmv2VersionEnum;


class ILodHeaderReader
{
public:
	virtual LodHeaderCommon ReadLodHeader(void*) = 0;
};

class LodHeaderFactory
{
	std::map<Rmv2VersionEnum, std::shared_ptr<ILodHeaderReader>> m_readerMap;

public:
	ILodHeaderReader* GetReader(Rmv2VersionEnum version);

	template <typename READER_TYPE>
	ILodHeaderReader* AddReader(Rmv2VersionEnum version);
		
	ILodHeaderReader* AddReader(Rmv2VersionEnum version, std::shared_ptr<ILodHeaderReader>);
};

class IMeshHeaderReader
{
public:
	virtual LodHeaderCommon ReaHeader(void*) = 0;

};

class ILodVertexReader
{
public:
	virtual CommonVertex ReadVertex(void*) = 0;
};

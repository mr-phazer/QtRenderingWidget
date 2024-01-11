#include "LodHeaderCreators.h"
#include "..\..\Helpers\ByteStream.h"

#include "..\RawStructs\RawLodHeaders.h"

using namespace rmv2::lod_header;

LODHeaderCommon LodHeader_V6_Creator::Create(ByteStream& bytes)
{			
	// TODO: pick which one to use, if any??
	 
	// raw_structs::LodHeaderElement_V5_V6 lodHeaderRaw;	
	// bytes.Read(&lodHeaderRaw, sizeof(raw_structs::LodHeaderElement_V5_V6));
	//LODHeaderCommon lodHeader;
	//lodHeader.m_dwMeshCount = lodHeaderRaw.dwMeshCount;
	//lodHeader.m_dwVerticesDataLength = lodHeaderRaw.dwVerticesDataLength;
	//lodHeader.m_dwIndicesDataLength = lodHeaderRaw.dwIndicesDataLength;
	//lodHeader.m_dwStartOffset = lodHeaderRaw.dwStartOffset;
	//lodHeader.m_fVisibilityDistance = lodHeaderRaw.fVisibilityDistance;	
	
	LODHeaderCommon lodHeader;

	lodHeader.m_dwMeshCount = bytes.GetElement<uint32_t>();
	lodHeader.m_dwVerticesDataLength = bytes.GetElement<uint32_t>();
	lodHeader.m_dwIndicesDataLength = bytes.GetElement<uint32_t>();
	lodHeader.m_dwStartOffset = bytes.GetElement<uint32_t>();
	lodHeader.m_fVisibilityDistance = bytes.GetElement<float>();

	return lodHeader;
}

size_t rmv2::lod_header::LodHeader_V6_Creator::GetHeaderSize()
{
	return 20U;
}

LODHeaderCommon LodHeader_V7_V8_Creator::Create(ByteStream& bytes)
{
	LODHeaderCommon lodHeader;

	lodHeader.m_dwMeshCount = bytes.GetElement<uint32_t>();
	lodHeader.m_dwVerticesDataLength = bytes.GetElement<uint32_t>();
	lodHeader.m_dwIndicesDataLength = bytes.GetElement<uint32_t>();
	lodHeader.m_dwStartOffset = bytes.GetElement<uint32_t>();
	lodHeader.m_fVisibilityDistance = bytes.GetElement<float>();
	lodHeader.m_dwAuthoredLodNumber = bytes.GetElement<uint32_t>();
	lodHeader.m_dwQualityLevel = bytes.GetElement<uint32_t>();

	return lodHeader;
}

size_t rmv2::lod_header::LodHeader_V7_V8_Creator::GetHeaderSize()
{
	return 28U;
}

LODHeaderCreatorFactory::LODHeaderCreatorFactory()
{
	Register(file_header::Rmv2VersionEnum::RMV2_V6, new LodHeader_V6_Creator);
	Register(file_header::Rmv2VersionEnum::RMV2_V7, new LodHeader_V7_V8_Creator);
	Register(file_header::Rmv2VersionEnum::RMV2_V8, new LodHeader_V7_V8_Creator);
}

#include "LodHeaderCreators.h"
#include "..\..\Helpers\ByteStream.h"
#include "..\DataTypes\MeshEnumsConstants.h"

#include "..\RawStructs\RawLodHeaders.h"

using namespace rmv2;

LODHeaderCommon LodHeader_V6_Creator::Create(ByteStream& bytes)
{			
	// TODO: pick which one to use, if any??
	 
	// raw_structs::LodHeaderElement_V5_V6 lodHeaderRaw;	
	// bytes.Read(&lodHeaderRaw, sizeof(raw_structs::LodHeaderElement_V5_V6));
	//LODHeaderCommon lodHeader;
	//lodHeader.dwMeshCount = lodHeaderRaw.dwMeshCount;
	//lodHeader.dwVerticesDataLength = lodHeaderRaw.dwVerticesDataLength;
	//lodHeader.dwIndicesDataLength = lodHeaderRaw.dwIndicesDataLength;
	//lodHeader.dwStartOffset = lodHeaderRaw.dwStartOffset;
	//lodHeader.fVisibilityDistance = lodHeaderRaw.fVisibilityDistance;	
	
	LODHeaderCommon lodHeader;

	lodHeader.dwMeshCount = bytes.GetElement<uint32_t>();
	lodHeader.dwVerticesDataLength = bytes.GetElement<uint32_t>();
	lodHeader.dwIndicesDataLength = bytes.GetElement<uint32_t>();
	lodHeader.dwStartOffset = bytes.GetElement<uint32_t>();
	lodHeader.fVisibilityDistance = bytes.GetElement<float>();

	return lodHeader;
}

size_t rmv2::LodHeader_V6_Creator::GetHeaderSize()
{
	return 20U;
}

LODHeaderCommon LodHeader_V7_V8_Creator::Create(ByteStream& bytes)
{
	LODHeaderCommon lodHeader;

	lodHeader.dwMeshCount = bytes.GetElement<uint32_t>();
	lodHeader.dwVerticesDataLength = bytes.GetElement<uint32_t>();
	lodHeader.dwIndicesDataLength = bytes.GetElement<uint32_t>();
	lodHeader.dwStartOffset = bytes.GetElement<uint32_t>();
	lodHeader.fVisibilityDistance = bytes.GetElement<float>();
	lodHeader.dwAuthoredLodNumber = bytes.GetElement<uint32_t>();
	lodHeader.qualityLevel = bytes.GetElement<LODHeaderCommon::QualityLevel>();

	return lodHeader;
}

size_t rmv2::LodHeader_V7_V8_Creator::GetHeaderSize()
{
	return 28U;
}

LODHeaderCreatorFactory::LODHeaderCreatorFactory()
{
	Register<Rmv2VersionEnum::RMV2_V6, LodHeader_V6_Creator>();
	Register<Rmv2VersionEnum::RMV2_V7, LodHeader_V7_V8_Creator>();
	Register<Rmv2VersionEnum::RMV2_V8, LodHeader_V7_V8_Creator>();
}

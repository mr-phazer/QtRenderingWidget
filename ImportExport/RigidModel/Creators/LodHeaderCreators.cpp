#include "LodHeaderCreators.h"
#include "..\..\Helpers\ByteStream.h"

using namespace rmv2::lod_header;

LODHeaderCommon LodHeader_V6_Creator::Create(ByteStream& bytes)
{
	LODHeaderCommon lodHeader;

	lodHeader.m_dwMeshCount = bytes.GetElement<uint32_t>();
	lodHeader.m_dwVerticesDataLength = bytes.GetElement<uint32_t>();
	lodHeader.m_dwIndicesDataLength = bytes.GetElement<uint32_t>();
	lodHeader.m_dwStartOffset = bytes.GetElement<uint32_t>();
	lodHeader.m_fVisibilityDistance = bytes.GetElement<float>();

	return lodHeader;
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

LODHeaderCreatorFactory::LODHeaderCreatorFactory()
{
	Register(file_header::Rmv2VersionEnum::RMV2_V6, new LodHeader_V6_Creator);
	Register(file_header::Rmv2VersionEnum::RMV2_V7, new LodHeader_V7_V8_Creator);
	Register(file_header::Rmv2VersionEnum::RMV2_V8, new LodHeader_V7_V8_Creator);
}

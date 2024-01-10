#include "FileHeaderCreators.h"

using namespace rmv2::file_header;

FileHeaderCommon FileHeaderCreatorDefault::Create(ByteStream& bytes)
{
    FileHeaderCommon fileHeader;

    fileHeader.versionFileHeader.dwSignature = bytes.GetElement<uint32_t>();
    fileHeader.versionFileHeader.modelVersion = bytes.GetElement<Rmv2VersionEnum>();
    fileHeader.wLodCount = bytes.GetElement<uint16_t>();
    fileHeader.unknown1 = bytes.GetElement<uint16_t>();
    bytes.Read(fileHeader.szSkeletonId, FileHeaderCommon::SKELETON_STRING_LENGTH);    

    return fileHeader;
}

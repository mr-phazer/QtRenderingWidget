#include "FileHeaderCreators.h"

using namespace rmv2;

FileHeaderCommon FileHeaderCreatorDefault::Create(ByteStream& bytes)
{
    FileHeaderCommon fileHeader;

    fileHeader.signature = bytes.GetElement<Rmv2FileSignatureEnum>();
    fileHeader.modelVersionId = bytes.GetElement<Rmv2VersionEnum>();
    fileHeader.wLodCount = bytes.GetElement<uint16_t>();
    fileHeader.unknown1 = bytes.GetElement<uint16_t>();
    bytes.Read(fileHeader.szSkeletonId, RMV2_SKELETON_LENGTH);      

    return fileHeader;
}

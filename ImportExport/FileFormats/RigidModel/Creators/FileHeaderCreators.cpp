#include "FileHeaderCreators.h"

using namespace rmv2;

FileHeaderCommon FileHeaderCreatorDefault::Create(ByteStream& bytes)
{
    FileHeaderCommon fileHeader;

    fileHeader.signature = bytes.TReadElement<Rmv2FileSignatureEnum>();
    fileHeader.modelVersionId = bytes.TReadElement<Rmv2VersionEnum>();
    fileHeader.wLodCount = bytes.TReadElement<uint16_t>();
    fileHeader.unknown1 = bytes.TReadElement<uint16_t>();
    bytes.Read(fileHeader.szSkeletonId, RMV2_SKELETON_LENGTH);      

    return fileHeader;
}

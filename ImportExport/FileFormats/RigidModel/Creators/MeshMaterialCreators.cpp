#include "MeshMaterialCreators.h"

#include "..\Constants\Rmv2Constants.h"

using namespace rmv2;
using namespace utils;

MaterialHeaderType5 MaterielHeaderType5Creator::Create(ByteStream& bytes)
{
    MaterialHeaderType5 meshHeader5;

	bytes.Read(&meshHeader5.vertexFormatId, sizeof(meshHeader5.vertexFormatId));		
	
	bytes.Read(&meshHeader5.szMeshName, RMV2_MAX_MESH_NAME);
	bytes.Read(meshHeader5.szTextureDirectory, RMV2_MAX_PATH);
	bytes.Read(meshHeader5.szFilters, RMV2_MAX_PATH);
	
	bytes.Read(&meshHeader5.paddingByte0, 1);
	bytes.Read(&meshHeader5.paddingByte1, 1);

	bytes.Read(&meshHeader5.transforms, sizeof(meshHeader5.transforms));	

	bytes.Read(&meshHeader5.iMatrixIndex, 4);
	bytes.Read(&meshHeader5.iParentMatrixIndex, 4);

	bytes.Read(&meshHeader5.dwAttachmentPointCount, 4);

	bytes.Read(&meshHeader5.dwTextureCount, 4);
	bytes.Read(&meshHeader5.materialParams, sizeof(meshHeader5.materialParams));

	bytes.Read(meshHeader5.szPadding, sizeof(meshHeader5.szPadding));	

	return meshHeader5;
}


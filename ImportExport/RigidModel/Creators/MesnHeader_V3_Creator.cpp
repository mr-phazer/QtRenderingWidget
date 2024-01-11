#include "MesnHeader_V3_Creator.h"

#include <DirectXMath.h>

using namespace rmv2::mesh_header;

MeshHeader_Type3_Common MeshHeader_Type3_Creator_V6_V7_V8::Create(ByteStream& bytes)
{
    MeshHeader_Type3_Common header;
    
	// TODO: CLEAN UP!! Dedice whether to use GetElement or Read
	header.RigidMaterialId = bytes.GetElement<RigidMaterialEnum>();

	//bytes.Read(&header.RigidMaterialId, 2);
	bytes.Read(&header.wRenderFlags, 2);
	bytes.Read(&header.dwMeshSectionSize, 4);

	bytes.Read(&header.dwVertexOffset, 4);
	bytes.Read(&header.dwVertexCount, 4);

	bytes.Read(&header.dwIndexOffset, 4);
	bytes.Read(&header.dwIndexCount, 4);;

	bytes.Read(&header.vMinBB, sizeof(DirectX::XMFLOAT3));
	bytes.Read(&header.vMaxBB, sizeof(DirectX::XMFLOAT3));

	bytes.Read(&header.szLightingConstants, 32);

	return header;   
}

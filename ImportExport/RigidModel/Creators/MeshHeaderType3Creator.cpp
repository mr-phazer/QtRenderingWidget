#include "MeshHeaderType3Creator.h"
#include <DirectXMath.h>


using namespace rmv2;

MeshHeaderType3 MeshHeaderType3Creator::Create(ByteStream& bytes)
{
    MeshHeaderType3 header;
    
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

#include "MeshHeaderType3Creator.h"
#include <DirectXMath.h>


using namespace rmv2;
using namespace utils;

MeshHeaderType3 MeshHeaderType3Creator::Create(ByteStream& bytes)
{
    MeshHeaderType3 meshHeaderType3;
    
	// TODO: CLEAN UP!! Dedice whether to use TReadElement or Read
	meshHeaderType3.RigidMaterialId = bytes.TReadElement<RigidMaterialEnum>();
	//bytes.Read(&meshHeaderType3.RigidMaterialId, 2);
	bytes.Read(&meshHeaderType3.wRenderFlags, 2);
	bytes.Read(&meshHeaderType3.dwMeshSectionSize, 4);

	bytes.Read(&meshHeaderType3.dwVertexOffset, 4);
	bytes.Read(&meshHeaderType3.dwVertexCount, 4);

	bytes.Read(&meshHeaderType3.dwIndexOffset, 4);
	bytes.Read(&meshHeaderType3.dwIndexCount, 4);;

	bytes.Read(&meshHeaderType3.vMinBB, sizeof(DirectX::XMFLOAT3));
	bytes.Read(&meshHeaderType3.vMaxBB, sizeof(DirectX::XMFLOAT3));

	bytes.Read(&meshHeaderType3.szLightingConstants, 32);	

	return meshHeaderType3;   
}


#include "CommonVertexType.h"


using namespace DirectX;

inline RlDx::CommonVertex::CommonVertex(XMFLOAT3 const& iposition, XMFLOAT3 const& inormal, XMFLOAT3 const& itangent, XMFLOAT3 const& ibitangent, XMFLOAT4 const& icolor, XMFLOAT2 const& itextureCoordinate) {
	SetVertexDataCommonVertex(iposition, inormal, itangent, ibitangent, icolor, itextureCoordinate);
}

inline void RlDx::CommonVertex::SetVertexDataCommonVertex(XMFLOAT3 const& iposition, XMFLOAT3 const& inormal, XMFLOAT3 const& itangent, XMFLOAT3 const& ibitangent, XMFLOAT4 const& icolor, XMFLOAT2 const& itextureCoordinate)
{
	position.x = iposition.x;
	position.x = iposition.y;
	position.x = iposition.z;
	position.w = 0;

	normal = inormal;
	tangent = itangent;
	bitangent = ibitangent;
	color = icolor;
	textureCoordinate = itextureCoordinate;
}

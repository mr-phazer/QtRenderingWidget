#include "..\..\..\Helpers\ByteStream.h"
#include "..\..\..\Helpers\DataConversion.h"
#include "..\..\RawStructs\VerticesRaw.h"
#include "VertexCreators.h"

#include "VertexCAHelpers.inl"

rldx::CommonVertex rmv2::Weighted2CommonVertexCreator::Create(ByteStream& bytes)
{	
	vertices_raw_packed::VertexWeighted2 rawVertex;
	bytes.Read(&rawVertex, vertices_raw_packed::VertexWeighted2::GetSize());

	rldx::CommonVertex outVertex;

	outVertex.position = CAVertexHelpers::DoVersion8Precision(FloatConverter::GetFloat4FromHalf4(rawVertex.position));

	// bone indices
	outVertex.boneIndices = 
	{
		rawVertex.boneIndices.x,
		rawVertex.boneIndices.y, 
		0, 
		0
	};

	// bone weights
	outVertex.weights.x = FloatConverter::GetUNORMFloatFromByte(rawVertex.boneWeights.x),
	outVertex.weights.y = 1.0f - outVertex.weights.x;
			
	// TBN
	outVertex.normal = FloatConverter::GetSNORMFloat3(rawVertex.normal);
	outVertex.tangent = FloatConverter::GetSNORMFloat3(rawVertex.tangent);
	outVertex.bitangent = FloatConverter::GetSNORMFloat3(rawVertex.bitangent);
	
	// texture coordinates
	outVertex.textureCoordinate = FloatConverter::GetFloat2FromHalf2(rawVertex.uv);
	
	return outVertex;
}

rldx::CommonVertex rmv2::DefaultCommonVertexCreator::Create(ByteStream& bytes)
{
	rldx::CommonVertex vertex;

	vertex.position = bytes.GetElement<DirectX::XMFLOAT4>();

	// 'bitangent' is stored before 'tangent' when VertexFormat = ´default`
	// and z and X are swapped
	vertex.normal = CAVertexHelpers::SwapZY(bytes.GetElement<DirectX::XMFLOAT3>());
	vertex.bitangent = CAVertexHelpers::SwapZY(bytes.GetElement<DirectX::XMFLOAT3>());
	vertex.tangent = CAVertexHelpers::SwapZY(bytes.GetElement<DirectX::XMFLOAT3>());

	vertex.textureCoordinate = bytes.GetElement<DirectX::XMFLOAT2>();

	return vertex;
}

rldx::CommonVertex rmv2::Weighted4CommonVertexCreator::Create(ByteStream& bytes)
{
	vertices_raw_packed::VertexWeighted4 rawVertex;
	bytes.Read(&rawVertex, vertices_raw_packed::VertexWeighted4::GetSize());

	rldx::CommonVertex outVertex;

	outVertex.position = CAVertexHelpers::DoVersion8Precision(FloatConverter::GetFloat4FromHalf4(rawVertex.position));

	outVertex.normal = FloatConverter::GetSNORMFloat3(rawVertex.normal);
	outVertex.tangent = FloatConverter::GetSNORMFloat3(rawVertex.tangent);
	outVertex.bitangent = FloatConverter::GetSNORMFloat3(rawVertex.bitangent);

	outVertex.textureCoordinate = FloatConverter::GetFloat2FromHalf2(rawVertex.uv);

	// bone indices
	outVertex.boneIndices =
	{
		rawVertex.boneIndices.x,
		rawVertex.boneIndices.y,
		rawVertex.boneIndices.x,
		rawVertex.boneIndices.w,
	};	
	
	outVertex.weights.x = FloatConverter::GetUNORMFloatFromByte(rawVertex.boneWeights.x);
	outVertex.weights.y = FloatConverter::GetUNORMFloatFromByte(rawVertex.boneWeights.y);
	outVertex.weights.z = FloatConverter::GetUNORMFloatFromByte(rawVertex.boneWeights.z);
	outVertex.weights.w = 1.0f - (outVertex.weights.x + outVertex.weights.y + outVertex.weights.z); // quick-dirty normalization

	return outVertex;
}
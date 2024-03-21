#include <Quantization\FloatCoverter.h>
#include "..\..\..\..\Helpers\ByteStream.h"
#include "..\..\RawStructs\VerticesRaw.h"
#include "VertexCreators.h"

#include "..\..\Helpers\VertexCAHelpers.inl"

rldx::CommonVertex rmv2::Weighted2CommonVertexCreator::Create(ByteStream& bytes, Rmv2VersionEnum rmv2Version)
{
	vertices_raw_packed::VertexWeighted2 rawVertex;
	bytes.Read(&rawVertex, vertices_raw_packed::VertexWeighted2::GetSize(rmv2Version));

	rldx::CommonVertex outVertex;

	outVertex.position = CAVertexHelpers::DoV8PrecisionOp(FloatConverter::GetFloat4FromHalf4(rawVertex.position));

	// bone indices
	outVertex.boneIndices =
	{
		rawVertex.boneIndices.x,
		rawVertex.boneIndices.y,
		0,
		0
	};

	// bone weights
	// TODO: RESTOR and REMOVE DEBUGGIN CODE
	//outVertex.weights.x = FloatConverter::GetUNORMFloatFromByte(rawVertex.boneWeights.x),
	outVertex.weights.x = FloatConverter::GetUNormFloat(rawVertex.boneWeights.x);

	outVertex.weights.y = 1.0f - outVertex.weights.x;

	// TBN
	outVertex.normal = FloatConverter::GetSNORMFloat3FromUByte4(rawVertex.normal);
	outVertex.tangent = FloatConverter::GetSNORMFloat3FromUByte4(rawVertex.tangent);
	outVertex.bitangent = FloatConverter::GetSNORMFloat3FromUByte4(rawVertex.bitangent);

	// texture coordinates
	outVertex.textureCoordinate = FloatConverter::GetFloat2FromHalf2(rawVertex.uv);

	return outVertex;
}

rldx::CommonVertex rmv2::DefaultCommonVertexCreator::Create(ByteStream& bytes, Rmv2VersionEnum rmv2Version)
{
	rldx::CommonVertex commonVertex;

	// half-float to float32 for each vector coord
	commonVertex.position = CAVertexHelpers::DoV8PrecisionOp(FloatConverter::GetFloat4FromHalf4(bytes.TReadElement<DirectX::PackedVector::XMUSHORT4>()));

	// half-float to float32 for each vector coord
	commonVertex.textureCoordinate = FloatConverter::GetFloat2FromHalf2(bytes.TReadElement<DirectX::PackedVector::XMUSHORT2>());
	auto textureCoordinate2 = FloatConverter::GetFloat2FromHalf2(bytes.TReadElement<DirectX::PackedVector::XMUSHORT2>());

	// -- For VertexFormat = "default", 'bitangent' is stored before 'tangent' and z and X are swapped in all vectors
	// - get each vector from 4 bytes -> SNORM float4
	commonVertex.normal = CAVertexHelpers::SwapZY(FloatConverter::GetSNORMFloat3FromUByte4(bytes.TReadElement<DirectX::PackedVector::XMUBYTE4>()));
	commonVertex.bitangent = CAVertexHelpers::SwapZY(FloatConverter::GetSNORMFloat3FromUByte4(bytes.TReadElement<DirectX::PackedVector::XMUBYTE4>()));
	commonVertex.tangent = CAVertexHelpers::SwapZY(FloatConverter::GetSNORMFloat3FromUByte4(bytes.TReadElement<DirectX::PackedVector::XMUBYTE4>()));

	// - get "color" from 4 bytes -> 4 UNOMRM float4
	commonVertex.color = FloatConverter::GetUNORMFloat4FromByte4(bytes.TReadElement<DirectX::PackedVector::XMUBYTE4>());

	return commonVertex;
}

rldx::CommonVertex rmv2::Weighted4CommonVertexCreator::Create(ByteStream& bytes, Rmv2VersionEnum rmv2Version)
{
	vertices_raw_packed::VertexWeighted4 rawVertex;
	bytes.Read(&rawVertex, vertices_raw_packed::VertexWeighted4::GetSize(rmv2Version));

	rldx::CommonVertex outVertex;

	outVertex.position = CAVertexHelpers::DoV8PrecisionOp(FloatConverter::GetFloat4FromHalf4(rawVertex.position));

	outVertex.normal = FloatConverter::GetSNORMFloat3FromUByte4(rawVertex.normal);
	outVertex.tangent = FloatConverter::GetSNORMFloat3FromUByte4(rawVertex.tangent);
	outVertex.bitangent = FloatConverter::GetSNORMFloat3FromUByte4(rawVertex.bitangent);

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


	if (rmv2Version == Rmv2VersionEnum::RMV2_V8)
	{
		outVertex.color = FloatConverter::GetUNORMFloat4FromByte4(rawVertex.color);
	}

	return outVertex;
}
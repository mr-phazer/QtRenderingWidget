#include <CommonLibs\Quantization\QuantTools.h>
#include "..\..\RawStructs\VerticesRaw.h"
#include <CommonLibs\Utils\ByteStream.h>
#include "VertexCreators.h"

#include "..\..\Helpers\VertexCAHelpers.inl"

using namespace utils;

rldx::CommonVertex rmv2::Weighted2CommonVertexCreator::Create(ByteStream& bytes, Rmv2VersionEnum rmv2Version)
{
	vertices_raw_packed::VertexWeighted2 rawVertex;
	bytes.Read(&rawVertex, vertices_raw_packed::VertexWeighted2::GetSize(rmv2Version));

	rldx::CommonVertex outVertex;

	outVertex.position = CAVertexHelpers::DoV8PrecisionOp(quant_tools::GetFloat4FromHalf4(rawVertex.position));

	// bone indices
	outVertex.boneIndices =
	{
		rawVertex.boneIndices.x,
		rawVertex.boneIndices.y,
		0,
		0
	};

	// bone weights	
	outVertex.weights.x = quant_tools::GetUNormFloat(rawVertex.boneWeights.x);
	outVertex.weights.y = 1.0f - outVertex.weights.x; // sum of weights must be 1.0f, and CA doesn't store weight 2

	// TBN
	outVertex.normal = quant_tools::GetSNORMFloat3FromUByte4(rawVertex.normal);
	outVertex.tangent = quant_tools::GetSNORMFloat3FromUByte4(rawVertex.tangent);
	outVertex.bitangent = quant_tools::GetSNORMFloat3FromUByte4(rawVertex.bitangent);

	// texture coordinates
	outVertex.textureCoordinate = quant_tools::GetFloat2FromHalf2(rawVertex.uv);

	return outVertex;
}

rldx::CommonVertex rmv2::DefaultCommonVertexCreator::Create(ByteStream& bytes, Rmv2VersionEnum rmv2Version)
{
	rldx::CommonVertex commonVertex;

	// half-float to float32 for each vector coord
	commonVertex.position = CAVertexHelpers::DoV8PrecisionOp(quant_tools::GetFloat4FromHalf4(bytes.TReadElement<DirectX::PackedVector::XMUSHORT4>()));

	// half-float to float32 for each vector coord
	commonVertex.textureCoordinate = quant_tools::GetFloat2FromHalf2(bytes.TReadElement<DirectX::PackedVector::XMUSHORT2>());
	auto textureCoordinate2 = quant_tools::GetFloat2FromHalf2(bytes.TReadElement<DirectX::PackedVector::XMUSHORT2>());

	// -- For VertexFormat = "default", 'bitangent' is stored before 'tangent' and z and X are swapped in all vectors
	// - get each vector from 4 bytes -> SNORM float4
	commonVertex.normal = CAVertexHelpers::SwapZY(quant_tools::GetSNORMFloat3FromUByte4(bytes.TReadElement<DirectX::PackedVector::XMUBYTE4>()));
	commonVertex.bitangent = CAVertexHelpers::SwapZY(quant_tools::GetSNORMFloat3FromUByte4(bytes.TReadElement<DirectX::PackedVector::XMUBYTE4>()));
	commonVertex.tangent = CAVertexHelpers::SwapZY(quant_tools::GetSNORMFloat3FromUByte4(bytes.TReadElement<DirectX::PackedVector::XMUBYTE4>()));

	// - get "color" from 4 bytes -> 4 UNOMRM float4
	commonVertex.color = quant_tools::GetUNORMFloat4FromByte4(bytes.TReadElement<DirectX::PackedVector::XMUBYTE4>());

	return commonVertex;
}

rldx::CommonVertex rmv2::Weighted4CommonVertexCreator::Create(ByteStream& bytes, Rmv2VersionEnum rmv2Version)
{
	vertices_raw_packed::VertexWeighted4 rawVertex;
	bytes.Read(&rawVertex, vertices_raw_packed::VertexWeighted4::GetSize(rmv2Version));

	rldx::CommonVertex outVertex;

	outVertex.position = CAVertexHelpers::DoV8PrecisionOp(quant_tools::GetFloat4FromHalf4(rawVertex.position));

	outVertex.normal = quant_tools::GetSNORMFloat3FromUByte4(rawVertex.normal);
	outVertex.tangent = quant_tools::GetSNORMFloat3FromUByte4(rawVertex.tangent);
	outVertex.bitangent = quant_tools::GetSNORMFloat3FromUByte4(rawVertex.bitangent);

	outVertex.textureCoordinate = quant_tools::GetFloat2FromHalf2(rawVertex.uv);

	// bone indices
	outVertex.boneIndices =
	{
		rawVertex.boneIndices.x,
		rawVertex.boneIndices.y,
		rawVertex.boneIndices.x,
		rawVertex.boneIndices.w,
	};

	outVertex.weights.x = quant_tools::GetUNORMFloatFromByte(rawVertex.boneWeights.x);
	outVertex.weights.y = quant_tools::GetUNORMFloatFromByte(rawVertex.boneWeights.y);
	outVertex.weights.z = quant_tools::GetUNORMFloatFromByte(rawVertex.boneWeights.z);
	outVertex.weights.w = 1.0f - (outVertex.weights.x + outVertex.weights.y + outVertex.weights.z); // quick-dirty normalization


	if (rmv2Version == Rmv2VersionEnum::RMV2_V8)
	{
		outVertex.color = quant_tools::GetUNORMFloat4FromByte4(rawVertex.color);
	}

	return outVertex;
}
#pragma once

#include "..\..\..\Helpers\ByteStream.h"
#include "..\..\..\Helpers\Templates.h"
#include "..\..\DataTypes\MeshEnumsConstants.h"
#include "..\..\QtRenderingWidget\Rldx\DataTypes\CommonVertex.h"

namespace rmv2 {

	/// <summary>
	/// Interface for creating vertex from byte stream
	/// </summary>
	class IVertexCreator
	{
	public:
		virtual rldx::CommonVertex Create(ByteStream& bytes) = 0;
	};

	/// <summary>
	/// DefaultCommonVertexCreator
	/// Unanimated vertex format, z and y are swapped, bitangent is stored before tangent
	/// </summary>
	class DefaultCommonVertexCreator : public IVertexCreator
	{
	public:
		rldx::CommonVertex Create(ByteStream& bytes) override;
	};

	/// <summary>
	/// Create common vertex, from CA VertexFormat = "Weighted/Weighted2" (2 weights)
	/// </summary>
	class Weighted2CommonVertexCreator : public IVertexCreator
	{
	public:
		rldx::CommonVertex Create(ByteStream& bytes) override;

	};

	/// <summary>
	/// Create common vertex, from CA VertexFormat = "Cinematic/Weighted4" (4 weights)
	/// </summary>
	class Weighted4CommonVertexCreator : public IVertexCreator
	{
	public:
		rldx::CommonVertex Create(ByteStream& bytes) override;
	};


	class VertexCreatorProvider : public helpers::TFactory<IVertexCreator, VertexFormatEnum>
	{
	public:
		VertexCreatorProvider()
		{
			Register(VertexFormatEnum::DefaultVertexFormat, new DefaultCommonVertexCreator());
			Register(VertexFormatEnum::WeightedVertexFormat, new Weighted2CommonVertexCreator());
			Register(VertexFormatEnum::CinematicVertexFormat, new Weighted4CommonVertexCreator());
		}
	};
};
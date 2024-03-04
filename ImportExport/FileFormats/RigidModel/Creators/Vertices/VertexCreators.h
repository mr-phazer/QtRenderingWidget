#pragma once

#include "..\..\..\..\Helpers\ByteStream.h"
#include "..\..\..\..\Helpers\Templates.h"
#include "..\..\Constants\Rmv2Constants.h"
#include "..\..\DataTypes\MeshEnumsConstants.h"
#include "..\..\Rldx\Rldx\DataTypes\CommonVertex.h"


namespace rmv2 {

	/// <summary>
	/// Interface for creating vertex from byte stream
	/// </summary>
	class IVertexCreator
	{
	public:
		virtual rldx::CommonVertex Create(ByteStream& bytes, Rmv2VersionEnum rmv2Version) = 0;
	};

	/// <summary>
	/// DefaultCommonVertexCreator
	/// Unanimated vertex format, z and y are swapped, bitangent is stored before tangent
	/// </summary>
	class DefaultCommonVertexCreator : public IVertexCreator
	{
	public:
		rldx::CommonVertex Create(ByteStream& bytes, Rmv2VersionEnum rmv2Version) override;
	};

	/// <summary>
	/// Create common vertex, from CA VertexFormat = "Weighted/Weighted2" (2 weights)
	/// </summary>
	class Weighted2CommonVertexCreator : public IVertexCreator
	{
	public:
		rldx::CommonVertex Create(ByteStream& bytes, Rmv2VersionEnum rmv2Version) override;
	};

	/// <summary>
	/// Create common vertex, from CA VertexFormat = "Cinematic/Weighted4" (4 weights)
	/// </summary>
	class Weighted4CommonVertexCreator : public IVertexCreator
	{
	public:
		rldx::CommonVertex Create(ByteStream& bytes, Rmv2VersionEnum rmv2Version) override;
	};

	class VertexCreatorFactory : public templates::TAbstractFactory<IVertexCreator, VertexFormatEnum>
	{
	public:
		VertexCreatorFactory()
		{
			Register<VertexFormatEnum::DefaultVertexFormat, DefaultCommonVertexCreator>();
			Register<VertexFormatEnum::WeightedVertexFormat, Weighted2CommonVertexCreator>();
			Register<VertexFormatEnum::CinematicVertexFormat, Weighted4CommonVertexCreator>();
		}

	public:
		virtual void HandleKeyNotFound(VertexFormatEnum keyValue)
		{
			throw std::exception(("ERROR: Model using unsported vertex format: Numerical Vertex Format ID: " + std::to_string(keyValue)).c_str());
		}

	};
};
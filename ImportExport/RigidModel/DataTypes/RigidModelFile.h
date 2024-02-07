#pragma once

#include <vector>

#include "FileHeader.h"
#include "LodHeader.h"
#include "MeshHeaderType3.h"
#include "MeshHeaderType5.h"
#include "MaterialBlockCommon.h"
#include "..\..\Rldx\Rldx\DataTypes\CommonVertex.h"

namespace rmv2 {

	struct MeshData16
	{
		std::vector<rldx::CommonVertex> vertices;
		std::vector<uint16_t> indices;
	};

	/// <summary>
	/// Contains 1 mesh with 1 material
	/// </summary>
	struct MeshBlockCommon
	{
		// TODO: remmove?
		//MeshHeaderType3 meshHeaderType3;
		//MaterialHeaderType5 meshHeaderType5;
		MeshHeaderType3 meshHeader;
		MaterialCommon materialBlock;
		MeshData16 meshData;
	};

	/// <summary>
	/// Contains the 1 or more sub-meshe(s) which make up a complete model
	/// </summary>
	struct ModelBlockCommon
	{
		std::vector<MeshBlockCommon> meshBlocks;
	};

	struct RigidModelFileCommon
	{
		FileHeaderCommon fileHeader;
		std::vector<LODHeaderCommon> lodHeaders;
		std::vector<ModelBlockCommon> lods;		
	};


} // namespace rmv2
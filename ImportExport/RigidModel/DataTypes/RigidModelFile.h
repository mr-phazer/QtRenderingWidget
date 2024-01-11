#pragma once

#include "FileHeader.h"
#include "LodHeader.h"
#include "MeshHeader_V3.h"

namespace rmv2 {
	
	

	// TODO: remove these!!!
	namespace mesh_header {
		class MeshHeader_v5_Common {};
	}	

	class AttachTable {};
	class MaterialBlock {};

	/// <summary>
	/// Contains 1 mesh with 1 material
	/// </summary>
	 
	struct MeshBlockCommon
	{
		mesh_header::MeshHeader_Type3_Common meshHeaderV3;
		mesh_header::MeshHeader_v5_Common meshHeaderV5;
		AttachTable attachTable;
		MaterialBlock material;
	};

	/// <summary>
	/// Contains the 1 or more sub-meshe(s) which make up a complete model
	/// </summary>
	struct ModelBlockCommon
	{
		std::vector<MeshBlockCommon> meshes;
	};


	struct RigidModelFileCommon
	{
		file_header::FileHeaderCommon fileHeader;
		std::vector<lod_header::LODHeaderCommon> lodHeaders;		
		std::vector<ModelBlockCommon> lods;
	};

} // namespace rmv2
#pragma once

#include "..\DataTypes\RigidModelFile.h"
#include "..\Creators\FileHeaderCreators.h"
#include "..\Creators\LodHeaderCreators.h"
#include "..\Creators\MesnHeader_V3_Creator.h"

namespace rmv2
{
	class RigidModelReader
	{
	public:
		RigidModelFileCommon Read(ByteStream& bytes)
		{
			RigidModelFileCommon rmv2File;

			ReadFileHeader(bytes, rmv2File);
			rmv2File.lodHeaders = ReadLodHeaders(bytes, rmv2File.fileHeader.modelVersionId, rmv2File.fileHeader.wLodCount);

			rmv2File.lods.resize(1);
			rmv2File.lods[0].meshes.resize(1);

			// ReadLods(bytes, rmv2File);
			ReadMeshBlock(bytes, rmv2File.fileHeader.modelVersionId, rmv2File.lods[0].meshes[0]);
			
			
			

			// TODO: IMPLEMENT REST

			return rmv2File;
		}
		

		void ReadFileHeader(ByteStream& bytes, RigidModelFileCommon& rmv2File)
		{
			file_header::FileHeaderCreatorDefault headerCreator;
			rmv2File.fileHeader = headerCreator.Create(bytes);
		}

		std::vector<lod_header::LODHeaderCommon> ReadLodHeaders(ByteStream& bytes, file_header::Rmv2VersionEnum rmv2VersionId, uint16_t wLODCount)
		{
			lod_header::LODHeaderCreatorFactory lodHeaderfactory;
			auto lodHeaderCreator = lodHeaderfactory.Get(rmv2VersionId);

			std::vector<lod_header::LODHeaderCommon> lodHeaders(wLODCount);

			for (size_t lodHeaderIndex = 0; lodHeaderIndex < wLODCount; lodHeaderIndex++) {
				lodHeaders[lodHeaderIndex] = lodHeaderCreator->Create(bytes);
			}

			return lodHeaders;
		}
				
		void ReadMeshBlock(ByteStream& bytes, file_header::Rmv2VersionEnum, MeshBlockCommon& meshblock)
		{
			mesh_header::MeshHeader_Type3_Creator_V6_V7_V8 meshHeaderType3Creator;
			meshblock.meshHeaderV3 = meshHeaderType3Creator.Create(bytes);

			/*
			 - read mesh header type 5
			 
			 - read attach table

			 - read material block  (Interface: IMaterialBlockReader/Creator)
			     read texture elementes
				 read extra material params
			 
			 - read mesh data
			   - read vertex data  (Interface: IVertexCreator)
			   - read index data
			 */

			// TODO: REMOVE
			auto DEBUG_1 = 1;
		}
	};
}
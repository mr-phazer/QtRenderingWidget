#pragma once

#include "..\DataTypes\RigidModelFile.h"
#include "..\Creators\FileHeaderCreators.h"
#include "..\Creators\LodHeaderCreators.h"

namespace rmv2
{
	class RigidModelReader
	{
	public:
		RigidModelFileCommon Read(ByteStream& bytes)
		{
			RigidModelFileCommon rmv2File;

			ReadFileHeader(bytes, rmv2File);
			ReadLodHeaders(bytes, rmv2File);
			// ReadLods(bytes, rmv2File);

			// TODO: IMPLEMENT REST

			return rmv2File;
		}

		void ReadFileHeader(ByteStream& bytes, RigidModelFileCommon& rmv2File)
		{
			file_header::FileHeaderCreatorDefault headerCreator;
			rmv2File.fileHeader = headerCreator.Create(bytes);
		}

		void ReadLodHeaders(ByteStream& bytes, RigidModelFileCommon& rmv2File)
		{
			lod_header::LODHeaderCreatorFactory lodHeaderfactory;
			auto lodHeaderCreator = lodHeaderfactory.Get(rmv2File.fileHeader.fileVersionHeader.modelVersion);

			rmv2File.lodHeaders.resize(rmv2File.fileHeader.wLodCount);

			for (size_t lodHeaderIndex = 0; lodHeaderIndex < rmv2File.lodHeaders.size(); lodHeaderIndex++) {
				rmv2File.lodHeaders[lodHeaderIndex] = lodHeaderCreator->Create(bytes);
			}
		}
	};
}
#pragma once

#include "..\DataTypes\RigidModelFile.h"
#include "..\Creators\FileHeaderCreators.h"
#include "..\Creators\LodHeaderCreators.h"

namespace rmv2
{
	class RigidModelReader
	{

	public:
		RigidModelFileCommon Read(file_helpers::ByteStream& in)
		{
			ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_karl_franz\emp_karl_franz.rigid_model_v2)");
			
			RigidModelFileCommon rmv2File;

			file_header::FileHeaderCreatorDefault headerCreator;
			rmv2File.fileHeader = headerCreator.Create(bytes);

			lod_header::LODHeaderCreatorFactory factory;
			auto lodHeaderCreator = factory.Get(file_header::Rmv2VersionEnum::RMV2_V6);
			
			rmv2File.lodHeaders.resize(rmv2File.fileHeader.wLodCount);
			for (size_t lodHeaderIndex = 0; lodHeaderIndex < rmv2File.lodHeaders.size(); lodHeaderIndex++)
			{
				rmv2File.lodHeaders[lodHeaderIndex] = lodHeaderCreator->Create(bytes);
			}
		}


	};

}
#pragma once

#include "..\DataTypes\LodHeader.h"

namespace rmv2 {
	namespace lod_header {

		class LodHeader_V6_Creator : public ILODHeaderCreator
		{
		public:
			LODHeaderCommon Create(ByteStream& bytes) override;
		};

		class LodHeader_V7_V8_Creator : public ILODHeaderCreator
		{
		public:
			LODHeaderCommon Create(ByteStream& bytes) override;
		};

		class LODHeaderCreatorFactory : public helpers::TFactory<ILODHeaderCreator, file_header::Rmv2VersionEnum>
		{
		public:
			LODHeaderCreatorFactory()
			{
				Register(rmv2::file_header::Rmv2VersionEnum::RMV2_V6, new lod_header::LodHeader_V6_Creator);
				Register(rmv2::file_header::Rmv2VersionEnum::RMV2_V7, new lod_header::LodHeader_V7_V8_Creator);
				Register(rmv2::file_header::Rmv2VersionEnum::RMV2_V8, new lod_header::LodHeader_V7_V8_Creator);
			}
		};

	}; // namespace lod_header
}; // namespace rmv2

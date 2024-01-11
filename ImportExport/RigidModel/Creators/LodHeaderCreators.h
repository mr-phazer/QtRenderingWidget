#pragma once

#include "..\DataTypes\LodHeader.h"

namespace rmv2 {
	namespace lod_header {		

		class LodHeader_V6_Creator : public ILODHeaderCreator
		{
		public:
			LODHeaderCommon Create(ByteStream& bytes) override;
			size_t GetHeaderSize() override;
		};

		class LodHeader_V7_V8_Creator : public ILODHeaderCreator
		{
		public:
			LODHeaderCommon Create(ByteStream& bytes) override;
			size_t GetHeaderSize() override;
		};

		class LODHeaderCreatorFactory : public helpers::TFactory<ILODHeaderCreator, file_header::Rmv2VersionEnum>
		{
		public:
			LODHeaderCreatorFactory();
		};

	}; // namespace lod_header
}; // namespace rmv2

#pragma once

#include "..\..\Helpers\ByteStream.h"

using ByteStream = ByteStream;

namespace rmv2 {
	namespace file_header {

		enum Rmv2VersionEnum : uint32_t
		{
			RMV2_V5 = 5,
			RMV2_V6 = 6,
			RMV2_V7 = 7,
			RMV2_V8 = 8,
			UNDEFINED = 0xFFFFFFFF
		};

		struct FileHeaderCommon
		{
			struct VersionFileHeader
			{
				constexpr static uint32_t RMV2_SIGNATURE = 0x32564D52;

				uint32_t dwSignature = 0;			// the 32 bit numberical eqauivalent for "RVM2" = 0x32564D52
				Rmv2VersionEnum modelVersion = UNDEFINED;		// 6 for Rome2/Atilla/Thrones, 7 for WH/WH2, 8 for 3K
			} 
			fileVersionHeader;


			// count is split up into two 16 biLPARAM lParams and the lower 16 bit is used,
			// because in some rare models the upper 16 bits ("G") is filled with some info		
			uint16_t wLodCount;
			uint16_t unknown1;

			static constexpr size_t SKELETON_STRING_LENGTH = 128;
			char szSkeletonId[SKELETON_STRING_LENGTH] = { 0 };
		};

		class IFileHeaderCreator
		{
		public:
			virtual FileHeaderCommon Create(ByteStream& bytes) = 0;
		};

	}; // namespace file_header
}; // namespace rmv2
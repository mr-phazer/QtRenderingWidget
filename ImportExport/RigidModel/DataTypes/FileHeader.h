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
		};

		// TODO: better than 'uint32_t RMV2_SIGNATURE = 0x32564D52' ?
		enum Rmv2FileSignatureEnum : uint32_t {	RMV2_ValidFile = 0x32564D52	};

		static constexpr size_t RMV2_SKELETON_STRING_LENGTH = 128;

		struct FileHeaderCommon
		{					
			Rmv2FileSignatureEnum signature = Rmv2FileSignatureEnum(0);			// the 32 bit numberical eqauivalent for "RVM2" = 0x32564D52
			Rmv2VersionEnum modelVersionId = Rmv2VersionEnum(0);		// 6 for Rome2/Atilla/Thrones, 7 for WH/WH2, 8 for 3K 
			
			// count is split up into two 16 biLPARAM lParams and the lower 16 bit is used,
			// because in some rare models the upper 16 bits ("G") is filled with some info		
			uint16_t wLodCount = 0;
			uint16_t unknown1 = 0;

			char szSkeletonId[RMV2_SKELETON_STRING_LENGTH] = { 0 };
		};


	}; // namespace file_header
}; // namespace rmv2
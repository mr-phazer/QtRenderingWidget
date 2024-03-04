#pragma once
#include <DirectXPackedVector.h>

#include "..\DataTypes\MeshEnumsConstants.h"

namespace vertices_raw_packed
{
	using ushort4 = DirectX::PackedVector::XMUSHORT4;
	using ushort2 = DirectX::PackedVector::XMUSHORT2;
	using ubyte4 = DirectX::PackedVector::XMUBYTE4;
	using ubyte2 = DirectX::PackedVector::XMUBYTE2;

	struct VertexWeighted2
	{
		static size_t GetSize(Rmv2VersionEnum rmv2Version)
		{
			switch (rmv2Version)
			{
				/*case Rmv2VersionEnum::RMV2_V5: return 32U;
				case Rmv2VersionEnum::RMV2_V6: return 32U;
				case Rmv2VersionEnum::RMV2_V7: return 32U;*/
			case Rmv2VersionEnum::RMV2_V8: return 32U;
			default: return 28U;

			}
		}

		ushort4 position;

		ubyte2 boneIndices;
		ubyte2 boneWeights;

		ubyte4 normal;
		ushort2 uv;

		ubyte4 tangent;
		ubyte4 bitangent;
		
		ubyte4 color; // RMV2 v8
	};

	struct VertexDefault
	{
		static size_t GetSize(Rmv2VersionEnum rmv2Version)
		{
			return 32U;

		//	switch (rmv2Version)
		//	{
		//		/*case Rmv2VersionEnum::RMV2_V5: return 32U;
		//		case Rmv2VersionEnum::RMV2_V6: return 32U;
		//		case Rmv2VersionEnum::RMV2_V7: return 32U;*/
		////	case Rmv2VersionEnum::RMV2_V8: return 36U;
		//	default: return 32U;

		//	}
		}

		ushort4 position;   // 8

		ushort2 uv1;   // 4
		ushort2 uv2;   // 4

		ubyte4 normal;   // 4
		ubyte4 bitangent; // 4
		ubyte4 tangent; //4

		ubyte4 color; // 4
	};

	struct VertexWeighted4
	{
		static size_t GetSize(Rmv2VersionEnum rmv2Version)
		{ 
			switch (rmv2Version)
			{
			/*case Rmv2VersionEnum::RMV2_V5: return 32U;
			case Rmv2VersionEnum::RMV2_V6: return 32U;
			case Rmv2VersionEnum::RMV2_V7: return 32U;*/
			case Rmv2VersionEnum::RMV2_V8: return 36U;
			default: return 32U;

			}
		}

		ushort4 position;
		ubyte4 boneIndices; // 8
		ubyte4 boneWeights; // 12

		ubyte4 normal; // 16

		ushort2 uv; // 20

		ubyte4 tangent;
		ubyte4 bitangent;

		ubyte4 color; // RMV2 v8
	};
};
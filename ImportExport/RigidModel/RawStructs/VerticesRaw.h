#pragma once

#include <DirectXPackedVector.h>

//using ushort4 = DirectX::PackedVector::XMUSHORT4;
//using ushort2 = DirectX::PackedVector::XMUSHORT2;
//using ubyte4 = DirectX::PackedVector::XMUBYTE4;
//using ubyte2 = DirectX::PackedVector::XMUBYTE2;
//#pragma pack(push)  /* push current alignment to stack */
//#pragma pack(1)     /* set alignment to 1 byte boundary */


struct ushort4
{
	uint16_t x, y, z, w;
};

struct ushort2
{
	uint16_t x, y;
};

struct ubyte4
{
	uint8_t x, y, z, w;
};

struct ubyte2
{
	uint8_t x, y;
};

//using ushort4 = uint8_t[8];
//using ushort2 = uint8_t[4];
//using ubyte4 = uint8_t[4];
//using ubyte2 = uint8_t[2];

namespace vertices_raw
{
	struct VertexWeighted2
	{
		static constexpr size_t GetSize() { return 28U; }

		ushort4 position;

		ubyte2 boneIndices; // 2
		ubyte2 boneWeights; // 2

		ubyte4 normal;
		ushort2 uv;

		ubyte4 tangent; // 4
		ubyte4 bitangent; // 4
	};

	struct VertexDefault
	{
		static constexpr size_t GetSize() { return 32U; }

		ushort4 position;   // 8

		ushort2 uv1;   // 4
		ushort2 uv2;   // 4

		ubyte4 normal;   // 4
		ubyte4 bitangent; // 4
		ubyte4 tangent; //4
	};

	struct VertexWeighted4
	{
		static constexpr size_t GetSize() { return 32U; }

		ushort4 position;
		ubyte4 boneIndices; // 8
		ubyte4 boneWeights; // 12

		ubyte4 normal; // 16

		ushort2 uv; // 20

		ubyte4 tangent;
		ubyte4 bitangent;		
	};
}

//#pragma pack(pop)   /* restore original alignment from stack */
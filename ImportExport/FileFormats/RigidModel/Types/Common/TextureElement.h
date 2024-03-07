#pragma once

#include <string>

#include "..\..\Constants\Rmv2Constants.h"

enum TextureTypeEnum : uint32_t
{
	eDiffuse = 0,
	eNormal = 1,
	eMask = 3,
	eAmbientOcclusion = 5,
	eTilingDirtUV2 = 7,
	eDirtAlphaMask = 8,
	eSkinMask = 10,
	eSpecular = 11,
	eGlossMap = 12,
	eDecalDirtmap = 13,
	eDecalDirtmask = 14,
	eDecalMask = 15,
	eDiffuseDamage = 17,

	// for model verion 8
	eBaseColor = 27,
	eMaterialMap = 29,

	// TODO: make texture->shader system more flexible,
	// maybe using RigidModelV2::TextureType::TextureTypeEnum as indexes to a vector of SRVs ?

	// these are "invented" texture type enum values, set be me for my clumsy rendering system
	eDecalDiffuse = eDiffuse + 30,
	eDecalNormal = eNormal + 30,
	eDecalSpecular = eSpecular + 30,

	// empty
	eTEX_ERROR_NOT_SET = 200,
	eEmpty = 400
};

namespace rmv2
{
	struct TextureElement
	{
		static size_t GetPathLength() { return RMV2_MAX_PATH; };

		TextureTypeEnum textureType = TextureTypeEnum(0);
		std::string texturePath = "";

	};
}
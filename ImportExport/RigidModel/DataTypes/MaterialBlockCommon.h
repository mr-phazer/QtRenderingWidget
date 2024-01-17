#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>

#include "TextureElement.h"
#include "AttachElement.h"

namespace rmv2 
{
	template <typename T>
	struct ExtraMaterialElement
	{
		int32_t id;
		T value;
	};

	struct ExtraMaterialParams
	{
		std::vector<ExtraMaterialElement<std::string>> stringParams;
		std::vector<ExtraMaterialElement<float>> floatParams;
		std::vector<ExtraMaterialElement<int32_t>> intParam;
		std::vector<ExtraMaterialElement<DirectX::XMFLOAT4>> float4Parmas;
	};

	struct MaterialBlockCommon
	{
		std::vector<AttachmentPointTableEntry_V6_V7_V8> attachElements;
		std::vector<TextureElement> textureElements;
		ExtraMaterialParams extraMaterialParams;
	};
}
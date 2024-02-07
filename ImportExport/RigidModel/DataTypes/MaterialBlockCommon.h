#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>

#include "TextureElement.h"
#include "AttachElement.h"
#include "MeshHeaderType5.h"

namespace rmv2 
{
	template <typename CONST_BUF_DATA_TYPE>
	struct ExtraMaterialElement
	{
		int32_t id;
		CONST_BUF_DATA_TYPE value;
	};

	struct ExtraMaterialParams
	{
		std::vector<ExtraMaterialElement<std::string>> stringParams;
		std::vector<ExtraMaterialElement<float>> floatParams;
		std::vector<ExtraMaterialElement<int32_t>> intParam;
		std::vector<ExtraMaterialElement<DirectX::XMFLOAT4>> float4Parmas;
	};

	struct MaterialCommon
	{	
		MaterialHeaderType5 materialHeader;
		std::vector<AttachmentPointTableEntry_V6_V7_V8> attachElements;
		std::vector<TextureElement> textureElements;
		ExtraMaterialParams extraMaterialParams;
	};
}
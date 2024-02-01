#include "IMaterialCommonCreators.h"

using namespace rmv2;

MaterialCommon DefaultMaterialCreator::Create(ByteStream& bytes)
{
 	MaterialCommon matBlock;
	
	matBlock.materialHeader = MaterielHeaderType5Creator().Create(bytes);

	ReadAttachPointTable(bytes, matBlock, matBlock.materialHeader);
	ReadTextures(bytes, matBlock, matBlock.materialHeader);
	ReadExtraMaterialParams(bytes, matBlock, matBlock.materialHeader);

	return matBlock;
}

template <typename CONST_BUF_DATA_TYPE>
static void ReadParams(ByteStream& bytes, std::vector<ExtraMaterialElement<CONST_BUF_DATA_TYPE>>& params, size_t size)
{
	params.resize(size);
	for (auto& param : params)
	{
		bytes.TAutoRead(&param.id);
		bytes.TAutoRead(&param.value);
	}
};

void rmv2::DefaultMaterialCreator::ReadExtraMaterialParams(ByteStream& bytes, rmv2::MaterialCommon& matBlock, const rmv2::MeshHeader5Common& header5)
{
	ReadParams(bytes, matBlock.extraMaterialParams.stringParams, header5.materialParams.stringParamCount);
	ReadParams(bytes, matBlock.extraMaterialParams.floatParams, header5.materialParams.floatParamCount);
	ReadParams(bytes, matBlock.extraMaterialParams.intParam, header5.materialParams.intParamCount);
	ReadParams(bytes, matBlock.extraMaterialParams.float4Parmas, header5.materialParams.float4ParamCount);
}

void rmv2::DefaultMaterialCreator::ReadTextures(ByteStream& bytes, rmv2::MaterialCommon& matBlock, const rmv2::MeshHeader5Common& header)
{
	matBlock.textureElements.resize(header.dwTextureCount);
	for (auto& textureElement : matBlock.textureElements)
	{
		bytes.Read(&textureElement, sizeof(textureElement));
	}
}

void rmv2::DefaultMaterialCreator::ReadAttachPointTable(ByteStream& bytes, rmv2::MaterialCommon& matBlock, const rmv2::MeshHeader5Common& header)
{
	matBlock.attachElements.resize(header.dwAttachmentPointCount);
	for (auto& attachElement : matBlock.attachElements)
	{
		bytes.Read(&attachElement, sizeof(attachElement));
	}
}


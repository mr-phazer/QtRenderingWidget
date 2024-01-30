#include "MaterialBlockCommonCreators.h"
#include "..\..\DataTypes\MaterialBlockCommon.h"


using namespace rmv2;

MaterialBlockCommon MaterialBlockCommonCreator::Create(ByteStream& bytes, const MeshHeaderType5& header5)
{
	MaterialBlockCommon matBlock;
	ReadAttachPointTable(bytes, matBlock, header5);
	ReadTextures(bytes, matBlock, header5);
	ReadExtraMaterialParams(bytes, matBlock, header5);

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


void rmv2::MaterialBlockCommonCreator::ReadExtraMaterialParams(ByteStream& bytes, rmv2::MaterialBlockCommon& matBlock, const rmv2::MeshHeaderType5& header5)
{
	ReadParams(bytes, matBlock.extraMaterialParams.stringParams, header5.materialParams.stringParamCount);
	ReadParams(bytes, matBlock.extraMaterialParams.floatParams, header5.materialParams.floatParamCount);
	ReadParams(bytes, matBlock.extraMaterialParams.intParam, header5.materialParams.intParamCount);
	ReadParams(bytes, matBlock.extraMaterialParams.float4Parmas, header5.materialParams.float4ParamCount);
}

void rmv2::MaterialBlockCommonCreator::ReadTextures(ByteStream& bytes, rmv2::MaterialBlockCommon& matBlock, const rmv2::MeshHeaderType5& header)
{
	matBlock.textureElements.resize(header.dwTextureCount);
	for (auto& textureElement : matBlock.textureElements)
	{
		bytes.Read(&textureElement, sizeof(textureElement));
	}
}

void rmv2::MaterialBlockCommonCreator::ReadAttachPointTable(ByteStream& bytes, rmv2::MaterialBlockCommon& matBlock, const rmv2::MeshHeaderType5& header)
{
	matBlock.attachElements.resize(header.dwAttachmentPointCount);
	for (auto& attachElement : matBlock.attachElements)
	{
		bytes.Read(&attachElement, sizeof(attachElement));
	}
}

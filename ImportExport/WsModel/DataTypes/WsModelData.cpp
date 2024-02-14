#include "WsModelData.h"

rmv2::XMLMaterialData& rmv2::WsModelData::AllocXmLMaterial(size_t lodIndex, size_t partIndex)
{
	// resize 2d vectors as needed
	if (lodIndex >= xmlMateriData.size()) {
		xmlMateriData.resize(lodIndex + 1);
	}

	if (partIndex >= xmlMateriData[lodIndex].size()) {
		xmlMateriData[lodIndex].resize(partIndex + 1);
	}

	return xmlMateriData[lodIndex][partIndex];
}

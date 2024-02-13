#pragma once
#include "DxModelNode.h"
#include "..\..\ImportExport\RigidModel\DataTypes\TextureElement.h"
#include "..\..\ImportExport\Libs\PugiXML\pugixml.hpp"

namespace rldx
{
	class Asset
	{
		std::wstring pathRigidModelFile;
		std::vector<rmv2::TextureElement> texures;
	};

	enum class VmdTagEnum
	{
		Slot,
		VariantMesh
	};

	struct VmdTag
	{
		VmdTagEnum vmdTag;
		std::wstring name;
		pugi::xml_node xmlnode;
	};


	class DxVariantMeshhNode : public DxModelNode
	{		
		Asset m_assets;
		VmdTag m_vmdTag;


	};

}
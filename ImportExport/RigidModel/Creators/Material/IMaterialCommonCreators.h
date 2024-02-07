#pragma once

#pragma once

#include "..\..\..\Helpers\ByteStream.h"
#include "..\..\..\Helpers\Templates.h"
#include "..\..\DataTypes\MaterialBlockCommon.h"
#include "..\..\DataTypes\MeshHeaderType5.h"
#include "..\..\Creators\MeshHeaderType3Creator.h"
#include "..\..\Creators\MeshMaterialCreators.h"
#include "..\..\DataTypes\MeshEnumsConstants.h"

namespace rmv2 {

	class IMaterialCommonCreator
	{
	public:
		virtual MaterialCommon Create(ByteStream& bytes) = 0;
	};

	class DefaultMaterialCreator : public IMaterialCommonCreator
	{
	public:
		MaterialCommon Create(ByteStream& bytes) override;
		
	private:		
		void ReadExtraMaterialParams(ByteStream& bytes, rmv2::MaterialCommon& matBlock, const rmv2::MaterialHeaderType5& header);
		void ReadTextures(ByteStream& bytes, rmv2::MaterialCommon& matBlock, const rmv2::MaterialHeaderType5& header);
		void ReadAttachPointTable(ByteStream& bytes, rmv2::MaterialCommon& matBlock, const rmv2::MaterialHeaderType5& header);
	};

	class MaterialCommonCreatorFactory : public helpers::TFactory<IMaterialCommonCreator, RigidMaterialEnum>
	{
	public:
		MaterialCommonCreatorFactory()
		{		
			Register < RigidMaterialEnum::default_material, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted_decal, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted_dirtmap, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted_decal_dirtmap, DefaultMaterialCreator>();
		}



		void HandleKeyNotFound(RigidMaterialEnum keyValue) override
		{
			throw std::exception("Unsuported RigidModelV2 version (Action: Reading Mesh header.");
		}

	};

}
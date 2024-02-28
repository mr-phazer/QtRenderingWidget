#pragma once

#pragma once

#include "..\..\..\Helpers\ByteStream.h"
#include "..\..\..\Helpers\Templates.h"
#include "..\..\Creators\MeshHeaderType3Creator.h"
#include "..\..\Creators\MeshMaterialCreators.h"
#include "..\..\DataTypes\MaterialBlockCommon.h"
#include "..\..\DataTypes\MeshEnumsConstants.h"
#include "..\..\DataTypes\MeshHeaderType5.h"

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

	class MaterialCommonCreatorFactory : public templates::TAbstractFactory<IMaterialCommonCreator, RigidMaterialEnum>
	{
	public:
		MaterialCommonCreatorFactory()
		{
			Register < RigidMaterialEnum::default_material, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted_decal, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted_dirtmap, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted_skin, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted_skin_decal, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted_skin_decal_dirtmap, DefaultMaterialCreator>();
			Register < RigidMaterialEnum::weighted_skin_dirtmap, DefaultMaterialCreator>();

			Register < RigidMaterialEnum::decal_dirtmap, DefaultMaterialCreator>();
		}

		void HandleKeyNotFound(RigidMaterialEnum keyValue) override
		{
			throw std::exception(("Error: Unsuported RigidMaterial: Numerical ID: " + std::to_string(keyValue)).c_str());
		}
	};
}
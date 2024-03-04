#pragma once

#include "..\..\..\Helpers\ByteStream.h"
#include "..\..\..\Helpers\Templates.h"
#include "..\DataTypes\FileHeader.h"
#include "..\DataTypes\LodHeader.h"

namespace rmv2 {

	class ILODHeaderCreator
	{
	public:
		virtual LODHeaderCommon Create(ByteStream& bytes) = 0;
		virtual size_t GetHeaderSize() = 0;
	};

	class LodHeader_V6_Creator : public ILODHeaderCreator
	{
	public:
		LODHeaderCommon Create(ByteStream& bytes) override;
		size_t GetHeaderSize() override;
	};

	class LodHeader_V7_V8_Creator : public ILODHeaderCreator
	{
	public:
		LODHeaderCommon Create(ByteStream& bytes) override;
		size_t GetHeaderSize() override;
	};

	class LODHeaderCreatorFactory : public templates::TAbstractFactory<ILODHeaderCreator, Rmv2VersionEnum>
	{
	public:
		LODHeaderCreatorFactory();

		void HandleKeyNotFound(Rmv2VersionEnum keyValue) override
		{
			throw std::exception("Unsuported RigidModelV2 version (Action: Reading LOD headers.");
		}
	};

}; // namespace rmv2

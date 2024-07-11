#pragma once

#include "..\..\ImportExport\FileFormats\RigidModel\Readers\RigidModelReader.h"
#include "..\..\ImportExport\FileFormats\WsModel\Reader\WsModelReader.h"
#include "..\..\ImportExport\Libs\PugiXML\pugixml.hpp"
#include "DxModelNode.h"

namespace rldx {

	enum class VMDTagEnum
	{
		INVALID = 0,
		Slot = 1,
		VariantMesh = 2,
		VariantMeshReference = 4,
	};

	struct FileExtensions {

		static constexpr auto RigidModelV2 = L"rigid_model_v2";
		static constexpr auto  WSmodel = L"WSMODEL";
		static constexpr auto  VariantMeshDef = L"variantmeshdefinition";
	};

	/// <summary>
	/// The STATE tage as string id, for search/Compine
	/// </summary>
	struct VMDTagStrings {

		static constexpr auto Slot = L"SLOT";
		static constexpr auto  VariantMesh = L"VARIANT_MESH";
		static constexpr auto  VariantMeshReference = L"VARIANT_MESH_REFERENCE";
	};

	/// <summary>
	///  const 
	/// </summary>
	struct VMDTagAttributes {

		static constexpr auto ImposterModel = L"imposter_model";
		static constexpr auto Model = L"model";
		static constexpr auto Name = L"name";
		static constexpr auto Definition = L"definition";
		static constexpr auto AttachPoint = L"attach_point";
		static constexpr auto Probaility = L"probability";

	};

	struct VMDNodeData
	{
		bool IsVariantMesh() const { return (tagType == VMDTagEnum::VariantMesh || tagType == VMDTagEnum::VariantMeshReference); }

		VMDTagEnum tagType = VMDTagEnum::INVALID;
		std::wstring tagName;

		struct VaritantMeshData
		{
			std::wstring imposterModelPath = L"";
			std::wstring modelPath = L"";
			rmv2::WsModelData wsModelData;
			std::unique_ptr<rmv2::RigidModelFileCommon> parsedRigidModelFile; // avoid having to load the file twice (tree.build and tree.allocate
		} varintMeshData;

		struct VaritantRefereceData
		{
			std::wstring externalVmdFilePath = L"";

		} varintMesReferencehData;

		struct SlotData
		{
			std::wstring m_nodeName = L"";
			std::wstring imposterModelPath = L"";
			std::wstring modelPath = L"";
			std::wstring attcachPointName;
			float probability = 1.0f;

		} slotData;
	};

	class DxVmdNode : public DxModelNode
	{
	public:
		VMDNodeData vmdNodeData;

		DxVmdNode() : DxModelNode(L"Unnamed DxVmdNode") {}
		DxVmdNode(const std::wstring& m_nodeName) : DxModelNode(m_nodeName) {}

	public:
		using UniquePtr = std::unique_ptr<DxVmdNode>;

	};

	class DxVMDSlotNode : public DxVmdNode
	{
	public:
		DxVMDSlotNode(const pugi::xml_node& xmlNode);
	};

	class DxVMDVariantMeshNode : public DxVmdNode
	{
	public:
		DxVMDVariantMeshNode(const pugi::xml_node& xmlNode);
	};
}

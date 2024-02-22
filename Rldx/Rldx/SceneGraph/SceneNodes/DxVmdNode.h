#pragma once

#include "DxModelNode.h"
#include "..\..\ImportExport\WsModel\Reader\WsModelReader.h"
#include "..\..\ImportExport\RigidModel\Readers\RigidModelReader.h"
#include "..\..\ImportExport\Libs\PugiXML\pugixml.hpp"

namespace rldx {

	enum class VMDTagEnum
	{
		INVALID = 0, 
		Slot,
		VariantMesh,
		VariantMeshReference		
	};

	struct FileExtensions {

		static constexpr auto RigidModelV2 = L"rigid_model_v2";
		static constexpr auto  WSmodel = L"WSMODEL";
		static constexpr auto  VariantMeshDef = L"variantmeshdefinition";
	};

	/// <summary>
	/// The STATE tage as string id, for search/Compine
	/// </summary>
	struct VMDTag {

		static constexpr auto Slot = L"SLOT";
		static constexpr auto  VariantMesh = L"VARIANT_MESH";
		static constexpr auto  VariantMeshReference = L"VARIANT_MESH_REFERENCE";
		static constexpr auto  AttachPoint = L"attach_point";
	};

	/// <summary>
	///  const 
	/// </summary>
	struct VMDTagAtrtibtes {

		static constexpr auto ImposterModel = L"imposter_model";
		static constexpr auto Model = L"model";
		static constexpr auto Name = L"name";
		static constexpr auto Definition = L"definition";
		static constexpr auto AttachPoint = L"attach_point";

	};

	struct VMDNodeData
	{
		VMDTagEnum Tag = VMDTagEnum::INVALID; 
		std::wstring Name;

		struct VaritantMeshData
		{
			std::wstring imposterModelPath = L"";
			std::wstring modelPath = L"";
			rmv2::WsModelData wsModelData;
		} varintMeshData;

		struct VaritantRefereceData
		{
			std::wstring externalVmdFilePath = L"";

		} varintMesReferencehData;

		struct SlotData
		{
			std::wstring name = L"";
			std::wstring imposterModelPath = L"";
			std::wstring modelPath = L"";
			std::wstring attcachPointName;
			float probality = 1.0f;

		} slotData;
	};

	class DxVmdNode : public DxModelNode
	{
		VMDNodeData m_vmdNodeData;

	public:
		using SharedPtr = std::shared_ptr<DxVmdNode>;
	public:						

		VMDNodeData& GetVMDNodeDataRef() { return m_vmdNodeData; }
		const VMDNodeData& GetVMDNodeDataRef() const { return m_vmdNodeData; }
	};


	class IDxVmdNodeAllocator
	{
	public:
		virtual void AllocateDxBuffers(ID3D11Device* poDevice, DxVmdNode* node) = 0;
		//virtual DxModelNode::SharedPtr AlloNode(ID3D11De	vice* poDevice, DxBaseNode* rootNode, ByteStream& data) = 0;

	};











}
	//class IDxVmdNodeCreator
	//{
	//public:
	//	/// <summary>
	//	/// Creates a VMD sceneGraphRoot from the XML, with tag, model path, and material
	//	/// But does not allocate any DX resources
	//	/// </summary>
	//	/// <param name="sceneGraphRoot"></param>
	//	/// <param name="xmlNode"></param>
	//	virtual DxVmdNode::SharedPtr Create(DxVmdNode* root, const pugi::xml_node& xmlNode) = 0;
	//	//virtual DxModelNode::SharedPtr AlloNode(ID3D11De	vice* poDevice, DxBaseNode* rootNode, ByteStream& data) = 0;
	//};






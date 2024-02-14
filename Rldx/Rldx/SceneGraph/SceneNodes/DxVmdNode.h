#pragma once

#include "DxModelNode.h"
#include "..\..\ImportExport\WsModel\DataTypes\WsModelData.h"

namespace rldx {

	enum class VMDTagEnum
	{
		Slot,
		VariantMesh,
		VariantMeshRef
	};

	namespace VMDTag {

		static constexpr auto Slot = L"SLOT";
		static constexpr auto VariantMesh = L"VARIANT_MESH";
		static constexpr auto VariantMeshRef = L"VARIANT_MESH_REFERENCE";
		static constexpr auto AttachPoint = L"attach_point";
		static constexpr auto Name = L" ";
	};

	struct VMDNodeInfo
	{
		VMDTagEnum Tag;
		std::wstring modelPath = L"";
		rmv2::WsModelData materialData;

	};

	class DxVmdNode : public DxModelNode
	{
		VMDNodeInfo nodeInfo;
	};


	class IDxVmdNodeAllocator
	{
	public:
		virtual void Allocate(ID3D11Device* poDevice, DxVmdNode* node) = 0;
		//virtual DxModelNode::SharedPtr AlloNode(ID3D11De	vice* poDevice, DxBaseNode* rootNode, ByteStream& data) = 0;

	};

	class IDxVmdNodeCreator
	{
	public:
		/// <summary>
		/// Creates a VMD node from the XML, with tag, model path, and material
		/// But does not allocate any DX resources
		/// </summary>
		/// <param name="node"></param>
		/// <param name="xmlNode"></param>
		virtual virtual DxVmdNode::SharedPtr  Create(DxVmdNode* node, const pugi::xml_node& xmlNode) = 0;
		//virtual DxModelNode::SharedPtr AlloNode(ID3D11De	vice* poDevice, DxBaseNode* rootNode, ByteStream& data) = 0;

	};


};
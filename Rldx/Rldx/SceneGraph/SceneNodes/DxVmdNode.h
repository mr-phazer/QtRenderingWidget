#pragma once

#include "DxModelNode.h"

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

	};

	class DxVmdNode : public DxModelNode
	{
		VMDNodeInfo nodeInfo;
	};

};
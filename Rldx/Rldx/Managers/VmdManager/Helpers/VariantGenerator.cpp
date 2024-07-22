#include "..\..\..\SceneGraph\SceneNodes\DxVariantMeshNode.h"
#include "VariantGenerator.h"

namespace rldx
{


	void VariantGenerator::GenerateVariant()
	{
		DisbaleAllNodes(m_sceneRootNode);
		GenerateNewVariant(m_sceneRootNode);
	}

	void VariantGenerator::DisbaleAllNodes(DxVmdNode* node)
	{
		node->SetDrawState(DxBaseNode::DrawStateEnum::DontDraw);

		for (auto& child : node->GetChildren())
		{
			auto vmdNode = static_cast<DxVmdNode*>(child.get());
			DisbaleAllNodes(vmdNode);
		}
	}

	void VariantGenerator::EnableVariantMesh(DxVmdNode* node)
	{
		node->SetDrawState(DxBaseNode::DrawStateEnum::Draw);

		for (auto& child : node->GetChildren()) // enable all sibling <SLOT>s inside the <VariantMesh>
		{
			auto vmdNode = static_cast<DxVmdNode*>(child.get());
			if (vmdNode->vmdNodeData.tagType == VMDTagEnum::Slot)
			{
				EnableSlot(vmdNode);
			}
		}
	}

	void VariantGenerator::EnableSlot(DxVmdNode* node)
	{
		node->SetDrawState(DxBaseNode::DrawStateEnum::Draw);

		if (node->GetChildCount() > 0)
		{
			auto childIndex = rand() % node->GetChildCount(); // Pick 1 "model" at random
			auto vmdNode = static_cast<DxVmdNode*>(node->GetChild(childIndex));

			vmdNode->SetDrawState(DxBaseNode::DrawStateEnum::Draw);
			m_poOutModels->push_back(vmdNode);

			EnableVariantMesh(vmdNode);
		}
	};

	inline void VariantGenerator::GenerateNewVariant(DxVmdNode* node)
	{
		if (node->vmdNodeData.tagType == VMDTagEnum::VariantMesh)
		{
			EnableVariantMesh(node);
		}
		else if (node->vmdNodeData.tagType == VMDTagEnum::Slot)
		{
			EnableSlot(node);
		}
	};
}
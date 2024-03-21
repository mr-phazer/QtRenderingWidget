#pragma once

#include "..\..\..\SceneGraph\SceneNodes\DxVmdNodes.h"

// TODO: move impl to .cpp, and forward declare as much as possible
namespace rldx
{
	/// <summary>
	/// Used to generate a "variant", 
	/// to randomize a full character from parts (header, torso, legs, weapons, etc),
	/// doing as the games do, following the "VMD RULES"
	/// </summary>
	class VariantGenerator
	{
		DxVmdNode* m_rootNode = nullptr;
		timer::SystemClockChecker m_clock; // used to random seed
	public:
		VariantGenerator(DxVmdNode* rootNode) : m_rootNode(rootNode) {}

		void GenerateVariant()
		{
			DisbaleAllNodes(m_rootNode);
			GenerateVariant(m_rootNode);
		}

	private:
		void DisbaleAllNodes(DxVmdNode* node)
		{
			node->SetDrawState(DxBaseNode::DrawStateEnum::DontDraw);

			for (auto& child : node->GetChildren())
			{
				auto vmdNode = static_cast<DxVmdNode*>(child.get());
				DisbaleAllNodes(vmdNode);
			}
		};

		void EnableVariantMesh(DxVmdNode* node)
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

		void EnableSlot(DxVmdNode* node)
		{
			node->SetDrawState(DxBaseNode::DrawStateEnum::Draw);
			if (node->GetChildCount() > 0) // enable only 1 <Variantmesh> inside the <SLOT>
			{
				auto childIndex = rand() % node->GetChildCount(); // Random variant
				auto vmdNode = static_cast<DxVmdNode*>(node->GetChild(childIndex));
				vmdNode->SetDrawState(DxBaseNode::DrawStateEnum::Draw);
				EnableVariantMesh(vmdNode);
			}
		}

		void GenerateVariant(DxVmdNode* node)
		{
			if (node->vmdNodeData.tagType == VMDTagEnum::VariantMesh)
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
			else if (node->vmdNodeData.tagType == VMDTagEnum::Slot)
			{
				node->SetDrawState(DxBaseNode::DrawStateEnum::Draw);
				if (node->GetChildCount() > 0) // enable only 1 <Variantmesh> inside the <SLOT>
				{
					auto childIndex = rand() % node->GetChildCount(); // Random variant
					auto vmdNode = static_cast<DxVmdNode*>(node->GetChild(childIndex));
					vmdNode->SetDrawState(DxBaseNode::DrawStateEnum::Draw);
					EnableVariantMesh(vmdNode);
				}
			}
		}
		//void EnableVmdNode(DxVmdNode* node)
		//{
		//	auto ticks = m_clock.GetSystemTick();
		//	std::srand(ticks);



		//	if (node->vmdNodeData.IsVariantMesh()) // if the node is a <variantmesh>, enable all its child <SLOT>s
		//	{
		//		EnableSlots(node);
		//	}

		//	else if (node->vmdNodeData.tagType == VMDTagEnum::Slot) // if the node is a <Slot>, enable ONE of its child <VariantMesh>s
		//	{
		//		EnableVariantMesh(node);
		//	}
		//	else
		//	{
		//		// TODO: REMOVE
		//		auto debug_1 = 1;
		//	}
		//}
	};
};

#pragma once

#include <time.h>
#include "..\..\..\SceneGraph\SceneNodes\DxVmdNode.h"

// TODO: move impl to .xpp, and forward declare as much as possible
namespace rldx
{
	class VariantGenerator
	{
		DxVmdNode* m_rootNode = nullptr;

	public:
		VariantGenerator(DxVmdNode* rootNode) : m_rootNode(rootNode) {}

		void GenerateVariant()
		{
			DisbaleAllNodes(m_rootNode);
			std::srand(time(nullptr));
			EnableVmdNode(m_rootNode);
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

		void EnableVmdNode(DxVmdNode* node)
		{
			libtools::SystemClock clock;

			auto ticks = clock.GetSysstemTick();
			std::srand(ticks);

			if (node->GetVMDNodeDataRef().Tag == VMDTagEnum::VariantMesh) // if the node is a <variantmesh>, enable all its child <SLOT>s
			{
				node->SetDrawState(DxBaseNode::DrawStateEnum::Draw);

				for (auto& child : node->GetChildren()) // enable all sibling <SLOT>s inside the <VariantMesh>
				{
					auto pVmdChildNode = static_cast<DxVmdNode*>(child.get());
					if (pVmdChildNode->GetVMDNodeDataRef().Tag == VMDTagEnum::Slot)
					{
						EnableVmdNode(pVmdChildNode);
					}
				}
			}
			else if (node->GetVMDNodeDataRef().Tag == VMDTagEnum::Slot) // if the node is a <Slot>, enable ONE of its child <VariantMesh>s
			{
				node->SetDrawState(DxBaseNode::DrawStateEnum::Draw);

				if (node->GetChildCount() > 0) // enable only 1 <Variantmesh> inside the <SLOT>
				{

					auto childIndex = rand() % node->GetChildCount(); // Random variant
					auto vmdNode = static_cast<DxVmdNode*>(node->GetChild(childIndex));
					EnableVmdNode(vmdNode);
				}
			}
		}
	};
}
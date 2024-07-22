#pragma once

#include "..\..\..\SceneGraph\SceneNodes\DxVmdNodes.h"

// TODO: move impl to .cpp, and forward declare as much as possible
namespace rldx
{
	class DxVariantMeshNode;
	/// <summary>
	/// Used to generate a "variant", 
	/// to randomize a full character from parts (header, torso, legs, weapons, etc),
	/// doing as the games do, following the "VMD RULES"
	/// </summary>
	class VariantGenerator
	{
		std::vector<DxModelNode*>* m_poOutModels = nullptr;
		DxVmdNode* m_sceneRootNode = nullptr;
		timer::SystemClockChecker m_clock; // used to random seed
	public:
		VariantGenerator(DxVmdNode* rootNode, std::vector<DxModelNode*>* poOutModels)
			: m_poOutModels(poOutModels), m_sceneRootNode(rootNode)
		{}

		void GenerateVariant();

	private:
		void DisbaleAllNodes(DxVmdNode* node);;
		void EnableVariantMesh(DxVmdNode* node);

		void EnableSlot(DxVmdNode* node);
		void GenerateNewVariant(DxVmdNode* node);
	};
};

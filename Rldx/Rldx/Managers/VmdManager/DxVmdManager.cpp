#include "..\..\Animation\Helpers\SkeletonHelpers.h"
#include "..\..\Creators\DxGameShaderCreators.h"
#include "..\..\Managers\DxDeviceManager.h"
#include "..\..\Managers\VmdManager\Helpers\DxMaterialInfoReader.h"
#include "..\..\Managers\VmdManager\Helpers\DxVmdAllocator.h"
#include "..\..\Managers\VmdManager\Helpers\VariantGenerator.h"
#include "..\..\SceneGraph\SceneNodes\DxDeformerNode.h"
#include "DxVmdManager.h"

using namespace utils;

namespace rldx
{
	void DxVmdManager::LoadVariantMeshIntoNode(DxBaseNode* poAssetRootNode, ByteStream& bytes, const std::wstring& gameIdString)
	{
		logging::LogAction(L"");

		m_deformerNode.reset(); // clear the deformer node, for new load

		DxBaseNode::RemoveChildrenRecursive(poAssetRootNode);
		m_sceneRootNode = poAssetRootNode;

		// -- build tree from xml, without loading any assets
		BuildTreeFromAssetFile(bytes);

		// -- load assets		
		WStringkeyMap<sm::Matrix> preTransformMap; // TODO: is this needed? Maybe it is as some RMV2 attach tabkes might contains non-indentity matrices
		AllocateTreeDXBuffers(gameIdString, preTransformMap);

		InitDeformation();

		auto upoNewVariantMeshNode = std::make_unique<DxVariantMeshNode>(L"VMD TEMP node", m_deformerNode.get());
		m_poVariantMeshNode = &m_sceneRootNode->AddChild(std::move(upoNewVariantMeshNode));
	}

	void DxVmdManager::GenerateNewVariant()
	{
		std::vector<DxModelNode*> outModels;
		VariantGenerator(m_vmdRootNode, &outModels).GenerateVariant();

		m_poVariantMeshNode->SetModels(outModels);
	}

	DxDeformerNode* DxVmdManager::GetDerformer()
	{
		return m_deformerNode.get();
	}

	void DxVmdManager::InitDeformation()
	{
		if (m_skeletonName.empty()) { // if not skeleton m_nodeName, no deformation stuff
			return;
		}

		m_deformerNode = DxDeformerNode::Create();
		m_deformerNode->LoadBindPose(skel_anim::GetPackPathFromSkeletonName(m_skeletonName));

		SetMeshDeformation(m_vmdRootNode);
		SetAttachPointsRecursive(m_vmdRootNode);
	}

	void DxVmdManager::BuildTreeFromAssetFile(ByteStream& bytes)
	{
		if (CompareExtension(bytes.GetPath(), FileExtensions::VariantMeshDef))
		{
			LoadFromVmdXML(bytes);
			return;
		}

		if (CompareExtension(bytes.GetPath(), FileExtensions::RigidModelV2))
		{
			LoadFromRigidModel(bytes);
			return;
		}

		if (CompareExtension(bytes.GetPath(), FileExtensions::WSmodel))
		{
			LoadFromWsmodelXML(bytes);
			return;
		}
	}

	/// <summary>
	/// Set the attach point m_nodeName for all childre of an attach node
	/// </summary>
	/// <param m_nodeName="node">Node to process</param>
	void SetAttachPointName(DxVmdNode* node)
	{
		if (!node) return;
		if (!node->GetParent()) return;

		auto parentNode = dynamic_cast<const DxVmdNode*>(node->GetParent());
		if (!parentNode) return;

		// TODO: use the ternary conditional operator, after testing
		if (node->vmdNodeData.slotData.attcachPointName.empty())
		{
			node->vmdNodeData.slotData.attcachPointName =
				parentNode->vmdNodeData.slotData.attcachPointName; // get the parent's attach point m_nodeName
		}
	}

	void DxVmdManager::AllocateTreeDXBuffers(const std::wstring& gameIdString, WStringkeyMap<sm::Matrix>& preTransformMap)
	{
		auto newPbrShaderCreator = GameShaderProgramCreatorFactory().Get(gameIdString);
		if (!newPbrShaderCreator) {
			throw std::exception("No shader program creator found for game");
		}

		auto newPbrShaderProgram = newPbrShaderCreator->Create(DxDeviceManager::Device());
		if (!newPbrShaderProgram) {
			throw std::exception("Error Creating Shader");
		}

		AllocateNodeDXBufferRecursive(m_vmdRootNode, newPbrShaderProgram, preTransformMap);
	}

	// TODO: REMOVE?
	//DxVmdNode::UniquePtr& DxVmdManager::GetNode() { return m_sceneRootNode; }

	void DxVmdManager::SetMeshDeformation(DxBaseNode* node)
	{
		node->SetDeformerNode(m_deformerNode.get(), -1);

		for (auto& childNode : node->GetChildren())
		{
			SetMeshDeformation(childNode.get());
		}
	}

	void DxVmdManager::SetAttachPointsRecursive(DxMeshNode* node)
	{
		auto vmdNode = dynamic_cast<DxVmdNode*>(node);

		if (vmdNode && m_deformerNode)
		{
			m_deformerNode->AttachWeapon(vmdNode, vmdNode->vmdNodeData.slotData.attcachPointName);
		}

		for (auto& childNode : node->GetChildren())
		{
			SetAttachPointsRecursive(static_cast<DxMeshNode*>(childNode.get()));
		}
	}

	void DxVmdManager::LoadFromRigidModel(ByteStream& bytes) {

		auto gameShaderProgramCreator = GameShaderProgramCreatorFactory().Get(DxResourceManager::Instance()->GetGameIdString());
		if (!gameShaderProgramCreator) {
			throw std::exception("Error loadeinfGame Shader");
		}

		auto newPbrShaderProgram = gameShaderProgramCreator->Create(DxDeviceManager::Device());
		if (!newPbrShaderProgram) {
			throw std::exception("Error Creating Shader");
		}

		auto upoNewVmdRootNode = std::make_unique<DxVmdNode>(L"VMD Root Node");
		m_vmdRootNode = &m_sceneRootNode->AddChild(std::move(upoNewVmdRootNode));

		VMDNodeData& nodeTagData = m_vmdRootNode->vmdNodeData;
		nodeTagData.tagType = VMDTagEnum::VariantMesh; // add the RMV2 as
		nodeTagData.varintMeshData.modelPath = bytes.GetPath();
		nodeTagData.varintMeshData.imposterModelPath = L"";
		DxMaterialInfoReader(&nodeTagData).Parse();
	}

	void DxVmdManager::LoadFromWsmodelXML(ByteStream& bytes)
	{
		auto gameShaderProgramCreator = GameShaderProgramCreatorFactory().Get(DxResourceManager::Instance()->GetGameIdString());
		if (!gameShaderProgramCreator) {
			throw std::exception("Error loadeinfGame Shader");
		}

		auto newPbrShaderProgram = gameShaderProgramCreator->Create(DxDeviceManager::Device());
		if (!newPbrShaderProgram) {
			throw std::exception("Error Creating Shader");
		}

		auto upoNewVmdRootNode = std::make_unique<DxVmdNode>(L"VMD Root Node");
		m_vmdRootNode = &m_sceneRootNode->AddChild(std::move(upoNewVmdRootNode));

		VMDNodeData& newData = m_vmdRootNode->vmdNodeData;
		newData.tagType = VMDTagEnum::VariantMesh;
		newData.varintMeshData.modelPath = bytes.GetPath();
		newData.varintMeshData.imposterModelPath = L"";

		DxMaterialInfoReader(&newData).Parse();
	}

	void DxVmdManager::LoadFromVmdXML(ByteStream& bytes)
	{
		auto xmlParseResult = m_xmlDoc.load_buffer(bytes.GetBufferPtr(), bytes.GetBufferSize());

		if (!xmlParseResult) {
			throw std::exception(("Create::Read(): XML error: " + std::string(xmlParseResult.description())).c_str());
		}

		auto upoNewVmdRootNode = std::make_unique<DxVmdNode>(L"VMD Root Node");
		m_vmdRootNode = &m_sceneRootNode->AddChild(std::move(upoNewVmdRootNode));

		m_vmdRootNode->vmdNodeData.tagType = VMDTagEnum::Slot;
		m_treeBuilder.Build(m_vmdRootNode, m_xmlDoc.first_child());
	}

	void DxVmdManager::AllocateNodeDXBufferRecursive(DxVmdNode* poVmdNode, DxMeshShaderProgram* shaderProgram, WStringkeyMap<sm::Matrix>& preTransformMap)
	{
		m_skeletonName = L""; // reset skeleton m_nodeName;
		DxVmdNodeAllocator(poVmdNode, shaderProgram).AllocateDxBuffers(m_skeletonName, preTransformMap);

		for (auto& itVmdNode : poVmdNode->GetChildren())
		{
			AllocateNodeDXBufferRecursive(static_cast<DxVmdNode*>(itVmdNode.get()), shaderProgram, preTransformMap);

			// grow the bounding box to contain the child node bounding box
			DirectX::BoundingBox::CreateMerged(
				poVmdNode->NodeBoundingBox(),
				poVmdNode->NodeBoundingBox(),
				itVmdNode.get()->NodeBoundingBox());
		}
	}
} // end namespace rldx

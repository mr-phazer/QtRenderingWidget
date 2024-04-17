#include <Rldx\Animation\Helpers\PathHelper.h>
#include "..\..\Creators\DxGameShaderCreators.h"
#include "..\..\Managers\DxDeviceManager.h"
#include "..\..\Managers\VmdManager\Helpers\DxMaterialInfoReader.h"
#include "..\..\Managers\VmdManager\Helpers\DxVmdAllocator.h"
#include "..\..\Managers\VmdManager\Helpers\VariantGenerator.h"
#include "..\..\SceneGraph\SceneNodes\DxDeformerNode.h"
#include "DxVmdManager.h"

namespace rldx
{
	void DxVmdManager::LoadVariantMesh(DxBaseNode* poSceneNode, ByteStream& bytes, const std::wstring& gameIdString)
	{
		m_rootNode = DxBaseNode::Create(L"VMD Root Node");
		//poSceneNode->AddChild(m_rootNode);


		BuildTreeFromXml(bytes);
		m_rootNode->AddChild(m_vmdNode);


		WStringkeyMap<sm::Matrix> preTransformMap;


		AllocateDXBuffers(gameIdString, preTransformMap);

		InitAttachPoints();

		/*if (!m_skeletonName.empty())
		{
			LoadBindPose();
			SetMeshDeformation(m_rootNode.get());
			SetAttachPointsRecursive(m_vmdNode.get());
		}*/

		//m_rootNode->Transform().SetTranslation({ 3.5f,0.0f,0.0f });

		//DxSceneGraph::UpdateNodes(m_rootNode.get(), 0.0f);

		//GenerateVariant();
	}

	std::shared_ptr<DxVariantMeshNode> DxVmdManager::GenerateVariant()
	{
		auto newVariantNode = std::make_shared<DxVariantMeshNode>(m_skeletonName);

		VariantGenerator(m_vmdNode.get(), newVariantNode.get()).GenerateVariant();

		newVariantNode->Init();

		return newVariantNode;
	}

	void DxVmdManager::InitAttachPoints()
	{
		m_deformerNode = DxDeformerNode::Create();
		m_deformerNode->LoadBindPose(path_helper::GetPathFromSkeletonName(m_skeletonName));

		SetMeshDeformation(m_rootNode.get());
		SetAttachPointsRecursive(m_vmdNode.get());
	}

	void DxVmdManager::BuildTreeFromXml(ByteStream& bytes)
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
	/// Set the attach point name for all childre of an attach node
	/// </summary>
	/// <param name="node">Node to process</param>
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
				parentNode->vmdNodeData.slotData.attcachPointName; // get the parent's attach point name
		}
	}

	void DxVmdManager::AllocateDXBuffers(const std::wstring& gameIdString, WStringkeyMap<sm::Matrix>& preTransformMap)
	{
		auto newPbrShaderCreator = GameShaderProgramCreatorFactory().Get(gameIdString);
		if (!newPbrShaderCreator) {
			throw std::exception("No shader program creator found for game");
		}

		auto newPbrShaderProgram = newPbrShaderCreator->Create(DxDeviceManager::Device());
		if (!newPbrShaderProgram) {
			throw std::exception("Error Creating Shader");
		}

		AllocateNodesRecursive(m_vmdNode.get(), newPbrShaderProgram, preTransformMap);
	}

	DxVmdNode::SharedPtr DxVmdManager::GetNode() { return m_vmdNode; }

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

		auto gameShaderProgramCreator = GameShaderProgramCreatorFactory().Get(DxResourceManager::Instance()->GetGameIdSting());
		if (!gameShaderProgramCreator) {
			throw std::exception("Error loadeinfGame Shader");
		}

		auto newPbrShaderProgram = gameShaderProgramCreator->Create(DxDeviceManager::Device());
		if (!newPbrShaderProgram) {
			throw std::exception("Error Creating Shader");
		}

		m_vmdNode = std::make_shared<DxVmdNode>();
		VMDNodeData& nodeTagData = m_vmdNode->vmdNodeData;
		nodeTagData.tagType = VMDTagEnum::VariantMesh; // add the RMV2 as
		nodeTagData.varintMeshData.modelPath = bytes.GetPath();
		nodeTagData.varintMeshData.imposterModelPath = L"";
		DxMaterialInfoReader(&nodeTagData).Parse();
	}

	void DxVmdManager::LoadFromWsmodelXML(ByteStream& bytes)
	{
		m_vmdNode = std::make_shared<DxVmdNode>(L"DxVmdNode");
		VMDNodeData& newData = m_vmdNode->vmdNodeData;
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

		m_vmdNode = std::make_shared<DxVmdNode>(L"VmdNode");
		m_vmdNode->vmdNodeData.tagType = VMDTagEnum::Slot;

		m_treeBuilder.Build(m_vmdNode.get(), m_xmlDoc.first_child());
	}

	void DxVmdManager::AllocateNodesRecursive(DxVmdNode* m_vmdRootNode, DxMeshShaderProgram* shaderProgram, WStringkeyMap<sm::Matrix>& preTransformMap)
	{
		DxVmdNodeAllocator(m_vmdRootNode, shaderProgram).AllocateDxBuffers(m_skeletonName, preTransformMap);

		for (auto& childNode : m_vmdRootNode->GetChildren())
		{
			AllocateNodesRecursive(static_cast<DxVmdNode*>(childNode.get()), shaderProgram, preTransformMap);

			// grow the bounding box to contain the child node bounding box
			DirectX::BoundingBox::CreateMerged(
				m_vmdRootNode->GetNodeBoundingBox(),
				m_vmdRootNode->GetNodeBoundingBox(),
				childNode.get()->GetNodeBoundingBox());
		}

		auto& DEBUG_BOUNDING_BOX = m_vmdRootNode->GetNodeBoundingBox();
	}

}

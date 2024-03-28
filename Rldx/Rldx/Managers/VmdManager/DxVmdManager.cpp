#include "..\..\SceneGraph\SceneNodes\DxDeformerNode.h"
#include "DxVmdManager.h"

namespace rldx
{
	void DxVmdManager::LoadVariantMesh(DxBaseNode* poSceneNode, ByteStream& bytes, const std::wstring& gameIdString)
	{
		m_rootNode = DxBaseNode::Create(L"VMD Root Node");
		poSceneNode->AddChild(m_rootNode);


		BuildTreeFromXml(bytes);
		m_rootNode->AddChild(m_vmdNode);


		WStringkeyMap<sm::Matrix> preTransformMap;

		// TODO store this class field?
		std::string skeletonName;

		AllocateDXBuffers(gameIdString, skeletonName, preTransformMap);

		LoadSkeleton(m_rootNode.get(), skeletonName);
		SetMeshDeformation(m_rootNode.get());

		SetAttachPoints(m_vmdNode.get());
		GenerateVariant();
	}

	std::wstring GetPathFromSkeletonName(std::string& skeletonName)
	{
		// TODO: maybe make string literals consts in a proper place, like the resource manager?
		return LR"(animations\skeletons\)" + libtools::string_to_wstring(skeletonName) + L".anim";
	}

	void DxVmdManager::LoadSkeleton(DxBaseNode* poSceneNode, std::string& skeletonName)
	{
		m_deformerNode = DxDeformerNode::Create();
		m_deformerNode->LoadBindPose(GetPathFromSkeletonName(skeletonName));

		// TODO: remove TEST CODE
		//m_deformerNode->LoadAnimClip(LR"(K:\Modding\WH2\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_05.anim)");

		// TODO: remove TEST CODE: trying to load an ANIM v8 into the loaded VMD
		m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_pistol\locomotion\hu1_swp_combat_walk_01.anim)");
		//m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\deaths\hu1_sws_death_01.anim)");

		poSceneNode->AddChild(m_deformerNode);
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

	void DxVmdManager::AllocateDXBuffers(const std::wstring& gameIdString, std::string& destSkeletonName, WStringkeyMap<sm::Matrix>& preTransformMap)
	{
		auto newPbrShaderCreator = GameShaderProgramCreatorFactory().Get(gameIdString);
		if (!newPbrShaderCreator) {
			throw std::exception("No shader program creator found for game");
		}

		auto newPbrShaderProgram = newPbrShaderCreator->Create(DxDeviceManager::Device());
		if (!newPbrShaderProgram) {
			throw std::exception("Error Creating Shader");
		}

		AllocateNodesRecursive(m_vmdNode.get(), newPbrShaderProgram, destSkeletonName, preTransformMap);
	}

	void DxVmdManager::GenerateVariant()
	{
		VariantGenerator(m_vmdNode.get()).GenerateVariant();
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

	void DxVmdManager::SetAttachPoints(DxMeshNode* node)
	{
		auto vmdNode = dynamic_cast<DxVmdNode*>(node);

		if (vmdNode)
		{
			m_deformerNode->AttachWeapon(vmdNode, vmdNode->vmdNodeData.slotData.attcachPointName);
		}
		else
		{
			auto DEBUG_BREAK_1 = 1;
		}

		for (auto& childNode : node->GetChildren())
		{
			SetAttachPoints(static_cast<DxMeshNode*>(childNode.get()));
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

		// TODO: REMOVE?
		//poScene->SetDefaultShaderProgram(newPbrShaderProgram);

		// TODO remove?
		//rmv2::RigidModelReader rigidModelFileReader;
		//auto rmv2File = rigidModelFileReader.Read(bytes);
		//vmdNode->SetMeshData(DxDeviceManager::Device(), rmv2File);
		//vmdNode->LoadMaterialDataFromRmv2(DxDeviceManager::Device(), rmv2File);
		//vmdNode->SetShaderProgram(newPbrShaderProgram);

		m_vmdNode = std::make_shared<DxVmdNode>();
		VMDNodeData& nodeTagData = m_vmdNode->vmdNodeData;
		nodeTagData.tagType = VMDTagEnum::VariantMesh;
		nodeTagData.varintMeshData.modelPath = bytes.GetPath();
		nodeTagData.varintMeshData.imposterModelPath = L"";
		DxMaterialInfoReader(&nodeTagData).Parse();
	}

	void DxVmdManager::LoadFromWsmodelXML(ByteStream& bytes)
	{
		m_vmdNode = std::make_shared<DxVmdNode>();
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

		m_vmdNode = std::make_shared<DxVmdNode>();
		m_vmdNode->vmdNodeData.tagType = VMDTagEnum::Slot;

		m_treeBuilder.Build(m_vmdNode.get(), m_xmlDoc.first_child());
	}

	void DxVmdManager::AllocateNodesRecursive(DxVmdNode* m_vmdRootNode, DxMeshShaderProgram* shaderProgram, std::string& destSkeletonName, WStringkeyMap<sm::Matrix>& preTransformMap)
	{
		DxVmdNodeAllocator(m_vmdRootNode, shaderProgram).AllocateDxBuffers(destSkeletonName, preTransformMap);

		for (auto& childNode : m_vmdRootNode->GetChildren())
		{
			AllocateNodesRecursive(static_cast<DxVmdNode*>(childNode.get()), shaderProgram, destSkeletonName, preTransformMap);
		}
	}

}

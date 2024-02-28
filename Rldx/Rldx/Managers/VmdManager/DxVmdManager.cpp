#include "DxVmdManager.h"

void rldx::DxVmdManager::LoadVariantMesh(ByteStream& bytes)
{
	m_vmdhBytes = bytes;

	if (CompareExtension(bytes.GetPath(), rldx::FileExtensions::VariantMeshDef))
	{
		LoadFromVmdXML(bytes);
		return;
	}

	if (CompareExtension(bytes.GetPath(), rldx::FileExtensions::RigidModelV2))
	{
		LoadFromRigidModel(bytes);
		return;
	}

	if (CompareExtension(bytes.GetPath(), rldx::FileExtensions::WSmodel))
	{
		LoadFromWsmodelXML(bytes);
		return;
	}
}

void rldx::DxVmdManager::AllocateDXBuffers()
{
	auto newPbrShaderCreator = GameShaderProgramCreatorFactory().Get(game_id_keys::KEY_WARHAMMER_3);
	if (!newPbrShaderCreator) {
		throw std::exception("No shader program creator found for game");
	}

	auto newPbrShaderProgram = newPbrShaderCreator->Create(DxDeviceManager::Device());
	if (!newPbrShaderProgram) {
		throw std::exception("Error Creating Shader");
	}

	AllocateNodesRecursive(m_vmdRootNode.get(), newPbrShaderProgram);
}

void rldx::DxVmdManager::GenerateVariant()
{
	VariantGenerator(m_vmdRootNode.get()).GenerateVariant();
}

rldx::DxVmdNode::SharedPtr rldx::DxVmdManager::GetNode() { return m_vmdRootNode; }

void rldx::DxVmdManager::LoadFromRigidModel(ByteStream& bytes) {

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
	//m_vmdRootNode->SetModelData(DxDeviceManager::Device(), rmv2File);
	//m_vmdRootNode->LoadMaterialDataFromRmv2(DxDeviceManager::Device(), rmv2File);
	//m_vmdRootNode->SetShaderProgram(newPbrShaderProgram);

	m_vmdRootNode = std::make_shared<DxVmdNode>();
	VMDNodeData& nodeTagData = m_vmdRootNode->VMDTagData();
	nodeTagData.tagType = VMDTagEnum::VariantMesh;
	nodeTagData.varintMeshData.modelPath = bytes.GetPath();
	nodeTagData.varintMeshData.imposterModelPath = L"";
	DxMaterialInfoReader(&nodeTagData).Parse();
}

void rldx::DxVmdManager::LoadFromWsmodelXML(ByteStream& bytes)
{
	m_vmdRootNode = std::make_shared<DxVmdNode>();
	VMDNodeData& newData = m_vmdRootNode->VMDTagData();
	newData.tagType = VMDTagEnum::VariantMesh;
	newData.varintMeshData.modelPath = bytes.GetPath();
	newData.varintMeshData.imposterModelPath = L"";

	DxMaterialInfoReader(&newData).Parse();
}

void rldx::DxVmdManager::LoadFromVmdXML(ByteStream& bytes)
{
	auto xmlParseResult = m_xmlDoc.load_buffer(bytes.GetBufferPtr(), bytes.GetBufferSize());

	if (!xmlParseResult) {
		throw std::exception(("Create::Read(): XML error: " + std::string(xmlParseResult.description())).c_str());
	}

	m_vmdRootNode = std::make_shared<rldx::DxVmdNode>();
	m_vmdRootNode->VMDTagData().tagType = VMDTagEnum::Slot;

	m_treeBuilder.Build(m_vmdRootNode.get(), m_xmlDoc.first_child());
}

void rldx::DxVmdManager::AllocateNodesRecursive(DxVmdNode* m_vmdRootNode, DxMeshShaderProgram* shaderProgram)
{
	DxVmdNodeAllocator(m_vmdRootNode, shaderProgram).AllocateDxBuffers();

	for (auto& childNode : m_vmdRootNode->GetChildren())
	{
		AllocateNodesRecursive(static_cast<DxVmdNode*>(childNode.get()), shaderProgram);
	}
}


#include "BaseNode.h"

#include "..\..\pch.h"

#include "..\..\lib3d.h"
#include "..\..\QtRME_GUI\render_resources.h"
#include "..\..\IAbstractNode.h"
#include "..\..\ImporterLib\FBX_Reader.h"
#include <functional>
#include "..\..\RenderQueue.h"
#include "..\..\Skeleton.h"
#include "..\..\DXMesh/DXMeshFactoryService.h"
#include "..\..\Scene.h"

//#include "IDrawableNode.h"

using namespace rendutil;
//rendutil::BaseNode::BaseNode(rendutil::BaseNode* _poRoot)
//{
//	poRoot = _poRoot;
//}

size_t rendutil::BaseNode::sm_LOD_to_render = 0;
bool rendutil::BaseNode::sm_bShowNormals = false;
bool rendutil::BaseNode::sm_bShowTangents = false;
bool rendutil::BaseNode::sm_bShowBitangents = false;
float rendutil::BaseNode::sm_fNormalLength = 0.1f;

//void rendutil::BaseNode::AddItemToRenderQueue(ID3D11DeviceContext* _poDC, rendutil::RendererQueue* _poRenderQueue)
//
//{
//	for (auto& DxMeshBuf : oMesh_LODS[sm_LOD_to_render].getMeshes())
//	{
//		if (m_NodeType == NodeTypeEnum::eSkeletonNode && m_poDeformerSkeleton)
//		{
//			DxMeshBuf->setDeformerSkeleton(m_poDeformerSkeleton);
//
//			DxMeshBuf->VSConstBuffer.Data.mWorld = nodeTransform.global.getTransform();
//
//			//m_oDrawabelItem[0].m_spoDeformer = m_spoDeformerSkeleton;
//			//itDrawAble.mWorld = this->nodeTransform.global.getTransform();
//			//DxMeshBuf->m_derformerSkeletonNodeId = this->m_dwDeformerSkeletonId;
//			DxMeshBuf->setNodeId(this->m_dwNodeId);
//			//itDrawAble.m_NodeType = this->attributes.nodeType;
//			DxMeshBuf->attributes.shaderState = this->attributes.shaderState;
//
//			// TODO: should the bere a m_bDraw in both NODE and DrawAbleItem?
//			//itDrawAble.m_bDrawItem = this->m_bDrawNode;
//		}
//
//		_poRenderQueue->addItem(DxMeshBuf.get());
//	}
//}
//

//void rendutil::BaseNode::rotateByQuaternion(const DirectX::SimpleMath::Quaternion _Orientation)
//{
//	m_qLocalOrientation *= _Orientation;
//}


void rendutil::BaseNode::setDeformerSkeleton(Skeleton* _spoSkeleton)
{
	if (!_spoSkeleton)
	{
		m_dwDeformerSkeletonId = 0;
		m_poDeformerSkeleton = nullptr;
		return;
	}

	m_dwDeformerSkeletonId = _spoSkeleton->getNodeId();
	m_poDeformerSkeleton = _spoSkeleton;
};

rendutil::BaseNode::BaseNode()
{
	//this->oMesh_LODS.lods().resize(4);
	//m_dwNodeId = sm_next_id++;
}

//void rendutil::BaseNode::setVisible(bool _bState)
//{
//	m_bVisible = _bState;
//}

void rendutil::BaseNode::setChildrenVisible(bool _bState)
{
	m_bChildrenVisible = _bState;
}

void rendutil::BaseNode::setNodeType(NodeTypeEnum _NodeType)
{
	attributes.nodeType = _NodeType;
}

//bool rendutil::BaseNode::isVisible() const
//{
//	return m_bVisible;
//}

bool rendutil::BaseNode::areChildrenVisible() const
{
	return m_bChildrenVisible;
}

//rendutil::BaseNode* rendutil::BaseNode::create(rendutil::Scene* _poScene, const std::string& _strName)
//{
//	auto po = new BaseNode();
//
//	//_poScene->vecObjects.push_back(shared_ptr<IObjecet>(po));
//ar
//	return po;
//}

shared_ptr<rendutil::BaseNode> rendutil::BaseNode::create(ID3D11Device* _poDXDevice, const std::string& _strName, NodeTypeEnum NodeType)
{
	auto po = std::make_shared<BaseNode>();
	//_poScene->addObject(po);
	po->init(_poDXDevice);
	po->m_strName = _strName + "__" + std::to_string(po->getNodeId());
	po->attributes.nodeType = NodeType;

	return po;
}

std::SPNode rendutil::BaseNode::create(ID3D11Device* _poDXDevice, const std::string& _strName, bool _bIsRootNode)
{
	auto po = create(_poDXDevice, _strName, NodeTypeEnum::eBaseNode);
	po->m_bIsRootNode = _bIsRootNode;

	return po;
}

//bool rendutil::BaseNode::makeCubeMesh(rendutil::Scene* _poScene)
//{
//	m_spoSceneGraphNodeCube = rendutil::DXMeshBufferFactory::create(_poScene->getDevice());
//
//	//m_spoCubeMesh = std::DXMeshBuffer::create(_poScene->getDevice());
//	//m_spoCubeMesh = DXMeshFactory::create();
//	m_spoSceneGraphNodeCube->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
//	m_spoSceneGraphNodeCube->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
//
//
//
//	/*m_spoCubeMesh->m_poPixelShader =
//		_poScene->getResources()->getPixelShader(Resources::EPixelShaders::eSolidColor);
//
//	m_spoCubeMesh->m_poVertexShader =
//		_poScene->getResources()->getVertexShader(Resources::EVertexShader::eSolid);
//
//	m_spoCubeMesh->m_poInputLayout =
//		_poScene->getResources()->getInputLayout(Resources::EVertexShader::eSolid);
//
//	m_spoCubeMesh->m_poRasterizerState =
//		_poScene->getResources()->getRasterizerState(Resources::ERasterizers::eSolid);*/
//
//	std::vector<ModelVertex> pv(sm_vecCubeVertices.size());
//
//	for (size_t i = 0; i < sm_vecCubeVertices.size(); i++)
//	{
//		memset(&pv[i], 0, sizeof(ModelVertex));
//		pv[i].position.x = sm_vecCubeVertices[i].x * 0.0125f;
//		pv[i].position.y = sm_vecCubeVertices[i].y * 0.0125f;
//		pv[i].position.z = sm_vecCubeVertices[i].z * 0.0125f;
//		//pv[i].position.w = 0.f;
//
//		pv[i].color = SimpleMath::Color(0, 100, 0, 255);
//	};
//
//	m_spoSceneGraphNodeCube->createVertexBuffer(_poScene->getDevice(), (UINT)(sm_vecCubeVertices.size() * (size_t)sizeof(ModelVertex)), pv);
//	
//
//	m_spoSceneGraphNodeCube->createIndexBuffer(_poScene->getDevice(), (UINT)(sm_vecCubeIndices.size() * (size_t)sizeof(uint16_t)), &sm_vecCubeIndices[0]);
//	m_spoSceneGraphNodeCube->setIndexCount((UINT)(sm_vecCubeIndices.size()));	
//	m_spoSceneGraphNodeCube->setShaderState(EShaderState::eNoMaterialsStatic);
//	m_spoSceneGraphNodeCube->attributes.nodeType = NodeTypeEnum::eGridNode;
//	m_spoSceneGraphNodeCube->attributes.shaderState = EShaderState::eNoMaterialsStatic;
//
//	attributes.nodeType = NodeTypeEnum::eGridNode;
//	attributes.shaderState = EShaderState::eNoMaterialsStatic;
//
//	m_bIsCubeVisible = true;
//
//	return true;
//}
//

bool rendutil::BaseNode::makeCubeMesh(ID3D11Device* _pDXDevice)
{
	m_spoSceneGraphNodeCube = rendutil::DXMeshBufferFactory::create(_pDXDevice);

	//m_spoCubeMesh = std::DXMeshBuffer::create(pDXDevice);
	//m_spoCubeMesh = DXMeshFactory::create();
	m_spoSceneGraphNodeCube->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	m_spoSceneGraphNodeCube->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;


	std::vector<ModelVertex> pv(sm_vecCubeVertices.size());

	for (size_t i = 0; i < sm_vecCubeVertices.size(); i++)
	{
		memset(&pv[i], 0, sizeof(ModelVertex));
		pv[i].position.x = sm_vecCubeVertices[i].x * 0.0425f;
		pv[i].position.y = sm_vecCubeVertices[i].y * 0.0425f;
		pv[i].position.z = sm_vecCubeVertices[i].z * 0.0425f;
		//pv[i].position.w = 0.f;

		pv[i].color = SimpleMath::Color(0, 100, 0, 255);
	};

	

	m_spoSceneGraphNodeCube->createVertexBuffer(_pDXDevice, (UINT)(sm_vecCubeVertices.size() * (size_t)sizeof(ModelVertex)), pv.data());
	

	m_spoSceneGraphNodeCube->createIndexBuffer(_pDXDevice, (UINT)(sm_vecCubeIndices.size() * (size_t)sizeof(uint16_t)), &sm_vecCubeIndices[0]);
	m_spoSceneGraphNodeCube->setIndexCount((UINT)(sm_vecCubeIndices.size()));	
	m_spoSceneGraphNodeCube->setShaderState(EShaderState::eNoMaterialsStatic);
	m_spoSceneGraphNodeCube->attributes.nodeType = NodeTypeEnum::eGridNode;
	m_spoSceneGraphNodeCube->attributes.shaderState = EShaderState::eNoMaterialsStatic;

	m_spoSceneGraphNodeCube->strName = "Cube: Node: " + std::to_string(this->getNodeId()) + ": " + this->getName();;

	attributes.nodeType = NodeTypeEnum::eGridNode;
	attributes.shaderState = EShaderState::eNoMaterialsStatic;

	BoundingBox::CreateFromPoints(
		m_spoSceneGraphNodeCube->m_boundingBox, 
		pv.size(), (XMFLOAT3*)pv.data(),								  
		sizeof(ModelVertex));


	


	//m_bIsCubeVisible = true;

	return true;
}

bool rendutil::BaseNode::updateBoundingBoxMesh(ID3D11Device* _pDXDevice)
{
	//if (oMesh_LODS.size() == 0) // no lods ?
	//	return false;

	//if (oMesh_LODS[0].size() == 0) // no submeshes for lod 1?
	//	return false;


	//for (const auto& itLod : oMesh_LODS.lods().getLodLevels())
	//{

	//	for (auto& itMesh : itLod.getMeshes())
	//	{

	//		if (!itMesh.GeomtryMeshes.BBmeshBuffer || !itMesh.GeomtryMeshes.BBmeshBuffer->isBufferDirty())
	//		{


	//			//if (oBoundindBoxMeshes.size() == 0 & oMesh_LODS.lods().getLodLevels() == 0)
	//			{


	//				auto& m_spoBoundBoxMesh = oMesh_LODS[0][0].GeomtryMeshes.BBmeshBuffer;



	//				//m_spoBoundBoxMesh = rendutil::DXMeshufferFactory::create(_pDXDevice);

	//				m_spoBoundBoxMesh = rendutil::DXMeshBufferFactory::create(_pDXDevice);
	//				m_spoBoundBoxMesh->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	//				m_spoBoundBoxMesh->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	//				/*m_spoBoundBoxMesh->m_poPixelShader =
	//					_poScene->getResources()->getPixelShader(Resources::EPixelShaders::eSolidColor);

	//				m_spoBoundBoxMesh->m_poVertexShader =
	//					_poScene->getResources()->getVertexShader(Resources::EVertexShader::eSolid);

	//				m_spoBoundBoxMesh->m_poInputLayout =
	//					_poScene->getResources()->getInputLayout(Resources::EVertexShader::eSolid);

	//				m_spoBoundBoxMesh->m_poRasterizerState =
	//					_poScene->getResources()->getRasterizerState(Resources::ERasterizers::eSolid);*/

	//				vector<ModelVertex> pv(sm_vecCubeVertices.size());

	//				for (size_t i = 0; i < sm_vecCubeVertices.size(); i++)
	//				{
	//					memset(&pv[i], 0, sizeof(ModelVertex));
	//					pv[i].position.x = sm_vecCubeVertices[i].x * 0.0125f;
	//					pv[i].position.y = sm_vecCubeVertices[i].y * 0.0125f;
	//					pv[i].position.z = sm_vecCubeVertices[i].z * 0.0125f;
	//					//pv[i].position.w = 0.f;

	//					pv[i].color = { 0.1f, 1.f, 0.1f, 1.0f };
	//				}

	//				m_spoBoundBoxMesh->createVertexBuffer(_pDXDevice, (UINT)(sm_vecCubeVertices.size() * (size_t)sizeof(ModelVertex)), (void*)pv.data(),
	//								D3D11_USAGE::D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE);				
	//									

	//				m_spoBoundBoxMesh->createIndexBuffer(_pDXDevice, (UINT)(sm_vecCubeIndices.size() * (size_t)sizeof(uint16_t)), &sm_vecCubeIndices[0]);

	//				m_spoBoundBoxMesh->setIndexCount((UINT)(sm_vecCubeIndices.size()));

	//				m_bBoundingBoxVisible = true;
	//			}

	//		}

	//	}

	//}

	return true;
}

bool rendutil::BaseNode::init(ID3D11Device * _pDXDevice) {
	//return allocateCube(_poScene);
	return makeCubeMesh(_pDXDevice);
}

const string& rendutil::BaseNode::getName()
{
	return  m_strName;
}

void rendutil::BaseNode::setName(const string& _strName)
{
	m_strName = _strName;
}

std::shared_ptr<rendutil::BaseNode> rendutil::BaseNode::GetParent()
{	
	return m_poParent.lock();
}

//void rendutil::BaseNode::render(rendutil::Scene* _poScene, rendutil::Shader* _poShader)
//{
//	updateGlobalTransform();
//	//XMStoreFloat4x4(&_poScene->m_VS_PerModel_ConstantBuffer.world, XMMatrixTranspose(nodeTransform.global.getTransform()));
//
//	/*_poScene->m_VS_PerModel_ConstantBuffer.mWorld = (nodeTransform.global.getTransform()).Transpose();
//	_poScene->m_VS_PerModel_ConstantBuffer.mProjection =
//		_poScene->getCamera().ProjectMatrixPerspective;
//
//	_poScene->update_VS_PerModelConstBuffer();*/
//
//	//draw(_poScene);
//
//	//for (auto& it : getChildren())
//	//{
//	//	if (it->visible())
//	//	{
//	//		it->render(_poScene, _poShader);
//	//	}
//	//}
//}

//void rendutil::BaseNode::draw(rendutil::Scene* _poScene, bool _bUseIternalShader)
//{
//	if (m_bIsCubeVisible)
//	{
//		drawBoundingBox(_poScene);
//	}
//}

bool rendutil::BaseNode::setCubeVisible(bool _bState)
{
	bool bOldState = m_bIsCubeVisible;
	m_bIsCubeVisible = _bState;
	return bOldState;
}

rendutil::BaseNode::operator bool()
{
	return m_bIsValid;
}

//void rendutil::BaseNode::transform(const DirectX::XMFLOAT4X4& _mMatrix)
//{
//	//XMMATRIX m = XMLoadFloat4x4(&_mMatrix);
//	SimpleMath::Matrix m = _mMatrix;
//
//	m_xmRotation = m * m_xmRotation * m_xmTranslation;
//}
//
//void rendutil::BaseNode::mulTranslate(const DirectX::XMFLOAT3& _vTranslation)
//{
//	m_xmTranslation = m_xmTranslation * XMMatrixTranslation(_vTranslation.x, _vTranslation.y, _vTranslation.z);
//}
//
//void rendutil::BaseNode::setTranslate(const DirectX::XMFLOAT3& _t)
//{
//	m_xmTranslation = XMMatrixTranslation(_t.x, _t.y, _t.z);
//}
//
//void rendutil::BaseNode::setTranslateMatrix(const DirectX::XMFLOAT4X4& _m)
//{
//	m_xmTranslation = _m;
//}
//
//void rendutil::BaseNode::setScale(const DirectX::XMFLOAT3& _s)
//{
//	m_xmScaling = XMMatrixScaling(_s.x, _s.y, _s.z);
//}
//
//void rendutil::BaseNode::setRotateQuaternion(const DirectX::XMFLOAT4& _qOrienation)
//{
//	m_qLocalOrientation = _qOrienation;
//}
//
//void rendutil::BaseNode::setRotateMatrix(const DirectX::XMFLOAT4X4& _mRotation)
//{
//	m_xmRotation = _mRotation;
//}

//void rendutil::BaseNode::setRotateMatrix(const DirectX::XMFLOAT4X4& _mOrienation)
//{
//	m_xmRotation =	_mOrienation;
//}

/// <summary>
/// Sets EULER rotation to ABSOLUTE values (radians)
/// </summary>
/// <param name="_v">: vector containing rotation for each axis</param>
//void rendutil::BaseNode::setRotationEuler(const DirectX::XMFLOAT3& _v)
//{
//	m_xmRotation =
//
//		(
//			XMMatrixRotationZ(_v.z) *
//			XMMatrixRotationY(_v.y) *
//			XMMatrixRotationX(_v.x)
//			);
//}

/// <summary>
/// "Adds" rotation to the node's existing rotation (radians)
/// </summary>
///// <param name="_v">: vector containing rotation for each axis</param>
//void rendutil::BaseNode::rotateEuler(const DirectX::XMFLOAT3& _v)
//{
//	m_xmRotation =
//		m_xmRotation *
//		(
//			XMMatrixRotationZ(_v.z) *
//			XMMatrixRotationY(_v.y) *
//			XMMatrixRotationX(_v.x)
//			);
//}
//
//void rendutil::BaseNode::rotateRoll(const DirectX::XMFLOAT3& _vRoll)
//{
//	m_xmRotation = XMMatrixRotationRollPitchYaw(_vRoll.x, _vRoll.x, _vRoll.x);
//}
//
//const DirectX::XMFLOAT4X4& rendutil::BaseNode::getGlobalTransform()
//{
//	//updateGlobalMatrix();
//	DirectX::XMStoreFloat4x4(&m_matrixTemp, DirectX::XMMatrixTranspose(nodeTransform.global.getTransform()));
//
//	return m_matrixTemp;
//}

bool rendutil::BaseNode::_allocateCube(rendutil::Scene* _poScene)
{
//	m_spoCubeMesh = _poScene->getResources()->getSubMeshData(Resources::ESubMeshData::eCubeMesh);

	if (!m_spoSceneGraphNodeCube) // not a valid pointer = not created = create!
	{
		m_spoSceneGraphNodeCube = rendutil::DXMeshBufferFactory::create(_poScene->getDevice());
		m_spoSceneGraphNodeCube->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		m_spoSceneGraphNodeCube->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		//m_spoCubeMesh->m_poPixelShader =
		//	_poScene->getResources()->getPixelShader(Resources::EPixelShaders::eSolidColor);

		//m_spoCubeMesh->m_poVertexShader =
		//	_poScene->getResources()->getVertexShader(Resources::EVertexShader::eSolid);

		//m_spoCubeMesh->m_poInputLayout =
		//	_poScene->getResources()->getInputLayout(Resources::EVertexShader::eSolid);

		//m_spoCubeMesh->m_poRasterizerState =
		//	_poScene->getResources()->getRasterizerState(Resources::ERasterizers::eSolid);

		ModelVertex* pv = new ModelVertex[sm_vecCubeVertices.size()];

		for (size_t i = 0; i < sm_vecCubeVertices.size(); i++)
		{
			memset(&pv[i], 0, sizeof(ModelVertex));
			pv[i].position.x = sm_vecCubeVertices[i].x * 0.0125f;
			pv[i].position.y = sm_vecCubeVertices[i].y * 0.0125f;
			pv[i].position.z = sm_vecCubeVertices[i].z * 0.0125f;
			//pv[i].position.w = 0.f;

			pv[i].color = { 0.1f, 1.f, 0.1f, 1.0f };
		}

		m_spoSceneGraphNodeCube->createVertexBuffer(_poScene->getDevice(), (UINT)(sm_vecCubeVertices.size() * (size_t)sizeof(ModelVertex)), pv);
		delete[] pv;

		m_spoSceneGraphNodeCube->createIndexBuffer(_poScene->getDevice(), UINT(sm_vecCubeIndices.size() * (size_t)sizeof(uint16_t)), &sm_vecCubeIndices[0]);

		m_spoSceneGraphNodeCube->setIndexCount((UINT)(sm_vecCubeIndices.size()));

		// store
		_poScene->getResources()->setSubMeshData(Resources::ESubMeshData::eCubeMesh, m_spoSceneGraphNodeCube);
	}

	return true;
}

void rendutil::BaseNode::makeLineRecursvice(rendutil::Scene* _poScene, vector<DirectX::XMFLOAT4>& vertices, vector<uint16_t>& indices)
{
	XMFLOAT4X4 mGlobal;
	XMStoreFloat4x4(&mGlobal, nodeTransform.getLocalTransform());

	XMFLOAT4 po;
	po.x = mGlobal._14;
	po.y = mGlobal._24;
	po.z = mGlobal._34;
	po.z = 1;

	vertices.push_back(po);

	auto offset = indices.size();
	for (size_t i = 0; i < getChildren().size(); i++)
	{
		XMStoreFloat4x4(&mGlobal, getChildren()[i]->nodeTransform.getLocalTransform());

		po.x = mGlobal._14;
		po.y = mGlobal._24;
		po.z = mGlobal._34;
		po.z = 1;

		vertices.push_back(po);

		// make line from parent to each child
		indices.push_back((uint16_t)(offset + 0));
		indices.push_back((uint16_t)(offset + i));
	}

	for (auto& it : getChildren())
	{
		it->makeLineRecursvice(_poScene, vertices, indices);
	}
}

//void rendutil::BaseNode::makeLines(rendutil::Scene* _poScene,
//	vector<XMFLOAT4> v, vector<uint16_t> i)
//{
//	static int vertex_index = 0;
//	static int index = 0;
//
//	ModelVertex* pv;
//
//	XMFLOAT4X4 mGlobal;
//	XMStoreFloat4x4(&mGlobal, nodeTransform.global.getTransform());
//
//
//
//
//	for (auto& it : getChildren())
//	{
//
//
//		it->renderQuad(_poScene);
//	}
//
//
//}



bool rendutil::BaseNode::getMeshBuffersToDraw(DXMeshBufferVector& tempMeshes)
{
	return true;  

	//// -------------------------
	//// -------------------------_meshBuffer, 
	//// -------------------------

	//
	//DXMeshBufferVector temp;
	//temp.clear();

	//if (!oMesh_LODS.size())
	//	return temp;			
	//			

	//for (auto& it : oMesh_LODS[0].getMeshes())
	//{
	//	temp.push_back(it.get());
	//}

	//return temp;
}

bool rendutil::BaseNode::getActiveMeshes_Private(DXMeshBufferVector& _meshes)
{
	_meshes.clear();

	if (m_spoSceneGraphNodeCube && sm_benableCubes && m_bIsCubeVisible)
	{
		m_spoSceneGraphNodeCube->m_bIncludeInCollisionCheck = true;
		m_spoSceneGraphNodeCube->setParentNode(this);
		m_spoSceneGraphNodeCube->strName = this->m_strName;
		_meshes.addMesh(m_spoSceneGraphNodeCube.get(), this);
	}	

	return getMeshBuffersToDraw(_meshes);
}

BaseNode* rendutil::BaseNode::searchRecursiveForString(BaseNode* _popArent, const string& _str)
{
	if (tools::lower(_popArent->m_strName) == tools::lower(_str)) // ís node found?
	{
		return _popArent;
	}

	for (auto& it : _popArent->getChildren()) // traverse the tree recursively.
	{
		searchRecursiveForString(it.get(), _str);
	}

	return nullptr;
}

BaseNode* rendutil::BaseNode::searchRecursiveForNode(const SPBaseNode& _spoNode)
{
	if (_spoNode.get() == this) // ís node found?
	{
		return _spoNode.get();
	}

	for (auto& it : _spoNode->getChildren()) // traverse the tree recursively.
	{
		searchRecursiveForNode(it);
	}

	return nullptr;
}

void rendutil::BaseNode::updateVectors(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDC, size_t lod, size_t mesh)
{
	//if (m_wpoDeformerSkeleton.expired())
	//{
	//	return;
	//}

	//auto spoSkeleton = dynamic_pointer_cast<Skeleton::sptrSkeleton>(m_wpoDeformerSkeleton.lock());
	//auto& _oMeshData = m_spoRMV2File->vecLodLevels[lod].vecMeshes[mesh].oMeshData;

	//RigidModelV2::Common::UnPackedMesh32 oNormals;

	//oNormals.vecVertices.clear();
	//oNormals.vecIndices.clear();

	//uint16_t tempIndex = 0;
	//for (size_t i = 0; i < _oMeshData.oUnpackedMesh.vecVertices.size(); i++)
	//{
	//	ModelVertex p1;
	//	ModelVertex p2;

	//	p1.position = _oMeshData.oUnpackedMesh.vecVertices[i].position; // normal start a vertex
	//	p2.position = _oMeshData.oUnpackedMesh.vecVertices[i].position +

	//		Vector4(
	//			_oMeshData.oUnpackedMesh.vecVertices[i].normal.x,
	//			_oMeshData.oUnpackedMesh.vecVertices[i].normal.y,
	//			_oMeshData.oUnpackedMesh.vecVertices[i].normal.z,
	//			0
	//		)
	//		/*_oMeshData.oUnpackedMesh.vecVertices[i].normal*/ * 0.1f; // positon = v + v_normal * len_scalar

	//	p1.color = { 0.f, 0.f, 1.f, 1.f };
	//	p2.color = { 1.f, 0.f, 0.f, 1.f };

	//	p1.bone_indices = _oMeshData.oUnpackedMesh.vecVertices[i].bone_indices;
	//	p1.bone_weights = _oMeshData.oUnpackedMesh.vecVertices[i].bone_weights;

	//	p2.bone_indices = _oMeshData.oUnpackedMesh.vecVertices[i].bone_indices;
	//	p2.bone_weights = _oMeshData.oUnpackedMesh.vecVertices[i].bone_weights;

	//	/*	p1.normal = Vector3(
	//			oUnpackedMesh.vecVertices[i].normal.x,
	//			oUnpackedMesh.vecVertices[i].normal.y,
	//			oUnpackedMesh.vecVertices[i].normal.z

	//		);

	//		p2.normal = Vector3(
	//			oUnpackedMesh.vecVertices[i].normal.x,
	//			oUnpackedMesh.vecVertices[i].normal.y,
	//			oUnpackedMesh.vecVertices[i].normal.z

	//		);*/

	//	oNormals.vecVertices.push_back(p1);
	//	oNormals.vecVertices.push_back(p2);

	//	oNormals.vecIndices.push_back(tempIndex);
	//	oNormals.vecIndices.push_back(tempIndex + 1);

	//	tempIndex += 2;
	//	/*oNormals.vecIndices[2 * i] = i;
	//	oNormals.vecIndices[(2 * i) + 1] = i + 1;*/
	//};

	///*if (oMesh_LODS[lod].vecspoSubMeshesNormals[mesh])
	//{
	//	oMesh_LODS[lod].vecspoSubMeshesNormals[mesh]->createMesh(
	//		_pDevice,
	//		oNormals.vecVertices,
	//		oNormals.vecIndices,
	//		D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	//	);
	//}
	//else*/
	//{
	//	auto spoMeshBuffer = oMesh_LODS[lod][mesh]; //.spoNormalsLineData;

	//	// lock VRAM, and copy INDEX data from RAM
	//	D3D11_MAPPED_SUBRESOURCE poBufferInfo;
	//	HRESULT hr = _pDC->Map(spoMeshBuffer->indexBuffer(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &poBufferInfo);
	//	assert(SUCCEEDED(hr));

	//	::memcpy(poBufferInfo.pData, oNormals.vecIndices.data(), oNormals.vecIndices.size() * sizeof(uint16_t));
	//	_pDC->Unmap(spoMeshBuffer->indexBuffer(), 0);

	//	// lock VRAM, and copy vertex data from RAM
	//	hr = _pDC->Map(spoMeshBuffer->vertexBuffer(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &poBufferInfo);
	//	assert(SUCCEEDED(hr));

	//	memcpy(poBufferInfo.pData, spoMeshBuffer->vertexBuffer(), oNormals.vecVertices.size() * sizeof(ModelVertex));
	//	_pDC->Unmap(spoMeshBuffer->vertexBuffer(), 0);
	//}
}

//void rendutil::BaseNode::setTransForm(const DirectX::XMFLOAT4X4& _m)
//{
//	m_xmLocalTransform = _m;
//	m_bLocalTransformDirty = false;
//}

//void rendutil::BaseNode::updateGlobalTransform()
//{
//	// TODO: support scale
//	/*m_xmLocalTransform = m_xmTranslation * m_xmRotation * m_xmScale;*/
//	//if (m_bLocalTransformDirty)
//	updateLocalMatrix();
//
//	if (getParent())
//	{
//		nodeTransform.global.getTransform() = m_xmLocalTransform * getParent()->m_xmLocalTransform;
//	}
//	else
//	{
//		nodeTransform.global.getTransform() = m_xmLocalTransform;
//	}
//
//
//}

//void rendutil::BaseNode::updateLocalMatrix()
//{
//	m_xmLocalTransform = m_xmTranslation * (m_xmOrientation * m_xmRotation) * m_xmScaling;
//	m_bLocalTransformDirty = false;
//}

void rendutil::BaseNode::updateGlobalMatricesRecursive()
{
	updateGlobalTransform();

	for (auto& it : getChildren())
	{
		it->updateGlobalMatricesRecursive();
	}
}

BaseNode* rendutil::BaseNode::getRoot()
{
	auto po = getParent();

	while (po)
	{
		auto newNode = po->getParent();

		if (!newNode)
			return po.get();

		po = newNode;
	}

	return po.get();
}

void rendutil::BaseNode::AddChild(SPBaseNode _poBaseNode)
{	
	_poBaseNode->m_poParent = shared_from_this();
	getChildren().push_back(_poBaseNode);
	
}

void rendutil::BaseNode::AppendChild(SPBaseNode _poBaseNode)
{
	auto poRoot = getRoot();

	if (poRoot)
		if (poRoot->searchRecursiveForNode(_poBaseNode))
		{
			auto debug_1 = 1;
		}

	// use the magical "shared_from_this" to create a tempMeshVector shared_ptr from the this ptr
	_poBaseNode->m_poParent = shared_from_this();
	getChildren().push_back(_poBaseNode);

}

void rendutil::BaseNode::InsertChild(size_t _index, SPBaseNode _poNode)
{
	// use the magical shared_from_this to create a tempMeshVector shared_ptr from the this ptr
	_poNode->m_poParent = shared_from_this();
	getChildren().insert(getChildren().begin() + _index, _poNode);
}

void rendutil::BaseNode::AppendChildren(vector<SPBaseNode>& _vecChildren)
{
	for (const auto& it : _vecChildren)
	{
		AppendChild(it);
	}
}

void rendutil::BaseNode::EraseChildren()
{
	getChildren().clear();
}

void rendutil::BaseNode::DeleteChildren(bool bRecursive)
{
	if (!bRecursive)
	{
		EraseChildren();
		return;
	}

	for (size_t i = 0; i < getChildren().size(); i++)
	{
		getChildren()[i]->DeleteChildren(bRecursive);
	}

	EraseChildren();
}

bool rendutil::BaseNode::RemoveFromParent()
{
	auto parent = getParent();

	if (!parent)
	{
		return false;
	}

	for (auto itNode = parent->getChildren().begin(); itNode != parent->getChildren().end(); ++itNode)
	{
		// is the node found in the parentøs children
		if (itNode->get() == this)
		{
			parent->getChildren().erase(itNode); // yes so remove it
			return true;
		}
	}

	return false;
}

bool rendutil::BaseNode::DeleteNode()
{
	DeleteChildren();

	auto parent = getParent();

	if (!parent)
		return false;

	auto result = RemoveFromParent();

	return result;
}

bool rendutil::BaseNode::DeleteNode(SPBaseNode& _spoNode)
{
	return _spoNode->deleteNode();
}

size_t rendutil::BaseNode::getChildCount()
{
	return getChildren().size();
}

shared_ptr<rendutil::BaseNode> rendutil::BaseNode::findFirstChild(const std::string& _strName)
{
	for (auto& it : getChildren())
	{
		if (it->m_strName == _strName)
			return it;
	}

	return nullptr;
}


BaseNode* rendutil::BaseNode::findNodeRecursive(const std::string& _strName)
{
	return searchRecursiveForString(this, _strName);
}

shared_ptr<rendutil::BaseNode> rendutil::BaseNode::getChildByIndex(size_t _index)
{
	if (_index >= getChildren().size())
		return nullptr;

	return getChildren()[_index];
}

void rendutil::BaseNode::clearChildren()
{
	for (auto& it : getChildren())
	{
		it->clearChildren();
	}

	getChildren().clear();
}

std::vector<DirectX::XMFLOAT3>  rendutil::BaseNode::sm_vecCubeVertices = {
	{-1, -1, -1}, {1, -1, -1, }, {1, 1, -1}, {-1, 1, -1},
	{-1, -1, 1}, {1, -1, 1 }, {1, 1, 1}, {-1, 1, 1}
};

std::vector<uint16_t> rendutil::BaseNode::sm_vecCubeIndices =
{ 0, 1,  1, 2,  2, 3,  3,0,   // side 1

4, 5,  5, 6,  6, 7,  7,4,   // side 1

0, 4,   1,5,   2,6,  3, 7 };  // connecting lines

struct vert
{
	float x, y, z;
	float u, v;
	float nx, ny, nz;
};

const std::vector<vert> g_quad =
{
	{
		-1, 0, -1,
		0.f, 0.f,
		0.f, 1.f, 0.f
	},

	{
		1, 0, -1,
		1.f, 0.f,
		0.f, 1.f, 0.f
	},

	{
		1, 0, 1,
		1.f, 1.f,
		0.f, 1.f, 0.f
	},

	{
		-1, 0, 1,
		0.f, 1.f,
		0.f, 1.f, 0.f
	}
};

uint32_t BaseNode::sm_next_id = 1;

std::vector<std::SPNode> BaseNode::sm_vecTestChildren = std::vector<std::SPNode>(0);


bool BaseNode::sm_benableCubes = false;
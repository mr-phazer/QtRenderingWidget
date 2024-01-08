#pragma once
#include "IDrawable.h"
#include "IBindable.h"
#include "Rldx\Rendering\DxConstBuffer.h"
#include "..\SceneGraph\SceneNodes\DxMeshNode.h"

namespace Rldx {

	// -- forward declarations
	class DxMesh;
	class DxMaterial;
	class DxMeshShaderProgram;

	

	//// TODO: move to separate file, it does not belong in "interfaces", as it is a concrete implementation
	//class DxRenderItemMesh : public IRenderQueueItem
	//{
	//	DxMesh* meshData = nullptr;
	//	DxMaterial* material = nullptr;
	//	DxMeshShaderProgram* shaderProgram = nullptr;
	//	DxDeformerData* derformerBuffer = nullptr; // TODO: obtaine during scene graph parsing, from the "DxMeshNode::pDerformerNode" field

	//	sm::Matrix worldMatrix = sm::Matrix::Identity;
	//	sm::Vector3 pivotPoint = sm::Vector3(0, 0, 0);

	//public:
	//	DxRenderItemMesh(
	//		DxMesh* mesh,
	//		DxMeshShaderProgram* shaderProgram,
	//		DxMaterial* material = nullptr,
	//		DxDeformerData* = nullptr,
	//		const sm::Matrix& worldMatrix = sm::Matrix::Identity,
	//		const sm::Vector3& pivotPoint = sm::Vector3(0, 0, 0)
	//	) :
	//		meshData(mesh),
	//		shaderProgram(shaderProgram),
	//		material(material),
	//		derformerBuffer(derformerBuffer),
	//		worldMatrix(worldMatrix),
	//		pivotPoint(pivotPoint) {}

	//	void BindToDC(ID3D11DeviceContext* poDeviceContext) override
	//	{
	//		shaderProgram->BindToDC(poDeviceContext);
	//	}

	//	void Draw(ID3D11DeviceContext* poDeviceContext) override
	//	{
	//		meshData->Draw(poDeviceContext);
	//	}
	//};

	class IRenderBucket : public IDrawable
	{
	public:
		virtual void AddItem(IRenderQueueItem* item) = 0;
		virtual void Clear() = 0;
	};

	// TODO: remove old version?
	//class IRenderBucket : public IDrawable
	//{
	//public:
	//	virtual void AddItem(const DxRenderItemMesh& item) = 0;
	//	virtual void Clear() = 0;
	//};
}
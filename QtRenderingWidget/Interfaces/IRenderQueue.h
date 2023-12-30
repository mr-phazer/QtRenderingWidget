#pragma once
#include "IDrawable.h"

namespace Rldx {

	// -- forward declarations
	class DxMesh;
	class DxMaterial;
	class DxMeshShaderProgram;

	struct DxRenderQueueItem
	{
		DxMesh* meshData = nullptr;
		DxMaterial* material = nullptr;
		DxMeshShaderProgram* shaderProgram = nullptr;
		sm::Matrix worldMatrix = sm::Matrix::Identity;
		sm::Vector3 pivotPoint = sm::Vector3(0, 0, 0);
	};

	class IDxRenderQueue : public IDrawable 
	{	
	public:		
		virtual void AddItem(const DxRenderQueueItem& item) = 0;
		virtual void Clear() = 0;		
	};
}
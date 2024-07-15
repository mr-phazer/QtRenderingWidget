#pragma once

// TODO: clean? are both math libs neeeded
#include <DirectXMath.h>
#include <SimpleMath.h>

#include <Rldx\Managers\DxDeviceManager.h>
#include "..\..\DataTypes\DxMeshData.h"
#include "..\..\Rendering\DxMaterial.h"
#include "..\..\Rendering\DxMeshRenderData.h"
#include "DxBaseNode.h"

namespace rldx {

	// forward decl	
	struct DxMeshRenderingData;
	class DxMeshShaderProg;
	class DxSourceDeformerNode;

	template <class T, class... _Types>
	std::shared_ptr<T> NewShared(_Types&&... _Args)
	{
		auto newHeapInstance = std::make_shared<T>(_Args);

		if (newHeapInstance == nullptr) throw std::bad_alloc();

		return newHeapInstance;
	}

	template <class T, class... _Types>
	std::unique_ptr<T> NewUnique(_Types&&... _Args)
	{
		auto newHeapInstance = std::make_unique<T>(_Args);

		if (newHeapInstance == nullptr) throw std::bad_alloc();

		return newHeapInstance;
	}

	class DxMeshNode : public DxBaseNode
	{
	protected:
		DxMeshRenderingData m_meshData; // make this a INodeAtrribute or similar, an interfaces, from which other attributs like Mesh/Model/etc, are derived from

	public:
		using UniquePtr = std::unique_ptr<DxMeshNode>;
	public:
		DxMeshNode() : DxBaseNode()
		{
			SetType(SceneNodeTypeEnum::MeshNode);
			SetTypeString(L"DxMeshNode");
		}

		DxMeshNode(const std::wstring& name)
		{
			DxMeshNode();
			SetName(name);
		}

		// TODO: maybe add d3d device as param here also?, the "global" d3d devices might make the code "spaghetti"
		static UniquePtr Create(const std::wstring& m_nodeName = L"");

		DxMeshRenderingData& MeshRenderData() { return m_meshData; }
		void Clone(DxMeshRenderingData& clone) const;

		void SetMeshData(const rldx::DxCommonMeshData& meshData, std::wstring meshName, sm::Matrix mWeaponMatrix = sm::Matrix::Identity);
		void SetMeshPivot(const sm::Vector3& pivot) { m_meshData.pivot = pivot; }
		void SetMeshWorldTransForm(const sm::Matrix& mWorld) { m_meshData.perMesh_VS_CB.data.mWeaponWorld = mWorld; }
		void SetMeshVisbilityDistance(float distance) { m_meshData.visibilityDistance = distance; }

		virtual void SetNodeWorldTransForm(const sm::Matrix& mWorld) { m_meshData.perMesh_VS_CB.data.mNodeWorld = mWorld; }
		virtual void SetDeformerNode(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex) override;
		virtual void SetAttachBoneAsParent();
		virtual void SetShaderProgram(DxMeshShaderProgram* shaderProgram);

		void SetMaterial(rldx::DxMaterial* pDxMaterial) { m_meshData.poMaterial = pDxMaterial; };

		void FlushToRenderBucket(IRenderBucket* pRenderQueue) override;

		virtual void Update(float time) override;
	};

	///// <summary>
	///// Gets data from fbxsdk::FbxMeshNode
	///// </summary>
	//class FBXMesNode_Interface
	//{
	//	List<Vertex> GetVertives()
	//	{
	//		// call FBX, as we already to
	//	}
	//
	//	List<ushort> GetIndices()
	//	{
	//		// call FBX, as we already to
	//	}
	//
	//	Matrix GetMeshTransform()
	//	{
	//		// call FBX, as we already to
	//	}
	//};
	//
	///// <summary>
	///// Gets data from fbxsdk::FbxScene
	///// </summary>
	//class FBXSceneInterFace
	//{
	//	float GetScaleFactorToMetter();
	//};

};
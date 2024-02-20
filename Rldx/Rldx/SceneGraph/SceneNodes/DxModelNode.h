#pragma once

#include "DxMeshNode.h"
#include "..\..\ImportExport\RigidModel\DataTypes\RigidModelFile.h"

namespace rldx
{
	class IRenderBucket;

	/// <summary>
	/// Holds multiple meshes make up a complete model.
	/// </summary>
	class DxModelNode : public DxMeshNode
	{

		size_t m_activeLod = 0;;
		std::vector<std::vector<DxMeshNode::SharedPtr>> m_lods;
	public:
		using SharedPtr = std::shared_ptr<DxModelNode>;

	public:
		void SetShaderProgram(DxMeshShaderProgram* shaderProgram);;
		void SetModelData(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2File);;
		
		// TODO add support for (parsed) WSMODEL
		void LoadMaterialDataFromRmv2(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2File);;
		virtual void FlushToRenderBucket(IRenderBucket* pRenderQueue) override;;

	private:
		void SetSingleMesh(
			ID3D11Device* poDevice,
			size_t iLod,
			size_t iMesh,
			const rmv2::MeshHeaderType3& meshHeader,
			const rmv2::MaterialHeaderType5& materialHeader,
			const rmv2::MeshBlockCommon& rmr2MeshData);
	};
};
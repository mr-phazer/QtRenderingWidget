#pragma once

#include "DxMeshNode.h"
#include "..\..\Interfaces\IRenderBucket.h"
#include "..\..\ImportExport\RigidModel\DataTypes\RigidModelFile.h"
#include "..\..\Helpers\DxMeshCreator.h"

namespace rldx
{
    /// <summary>
    /// Holds multiple meshes make up a complete model.
    /// </summary>
    class DxModelNode : public DxBaseNode
    {
        size_t m_activeLod = 0;;
        std::vector<std::vector<DxMeshNode::SharedPointer>> m_lods =
        {
            {DxMeshNode::SharedPointer()}
        };

    public:
        void SetMeshNodes(size_t lodIndex, std::vector<std::vector<DxMeshNode::SharedPointer>> lods)
        {
            m_lods = lods;
        };

        void SetShaderProgram(DxMeshShaderProgram* shaderProgram)         
        { 
            for (auto& lod : m_lods)
            {
                for (auto& meshNode : lod)
                {
					meshNode->SetShaderProgram(shaderProgram);
				};
			};
        };

        void SetMeshData(ID3D11Device* poDevice,  const rmv2::RigidModelFileCommon& rmv2File )
        {
            size_t iLod = 0;
            m_lods.resize(1);
            m_lods[iLod].resize(rmv2File.lodHeaders[0].dwMeshCount);

            for (size_t iMesh = 0; iMesh < m_lods[0].size(); iMesh++)
            {
                m_lods[iLod][iMesh] = rldx::DxMeshNode::Create("Mesh");
                auto rm2MeshData = rldx::DxMeshCreatorHelper::CreateFromRmv2Mesh(poDevice, rmv2File.lods[iLod].meshBlocks[iMesh]);
                m_lods[iLod][iMesh]->SetMeshData(rm2MeshData);
            }                                 
        };

    public:
        virtual void FlushToRenderQueue(IRenderBucket* pRenderQueue) override
        {
            for (auto& itMeshNode : m_lods[m_activeLod])
            {
                itMeshNode->FlushToRenderQueue(pRenderQueue);
            };
        };
    };
};

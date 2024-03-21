#pragma once

// TODO: make as many forward declarations as possible
#include <FileFormats\Anim\Reader\TwAnimReader.h>
#include "..\..\Animation\AnimationPlayer.h"
#include "..\..\DataTypes\ConstBuffers\CPUConstBuffers.h"
#include "..\..\Helpers\DxMeshCreatorHelper.h"
#include "..\..\Managers\DxDeviceManager.h"
#include "..\..\Rendering\DxShaderProgram.h"
#include "DxMeshNode.h"

namespace rldx
{
	class DxDeformerNode : public DxMeshNode
	{
		skel_anim::AnimationPlayer m_animationPlayer;
		anim_file::TwAnimFileReader m_animFileReader;

	public:
		static std::shared_ptr<DxDeformerNode> Create(const std::wstring& name = L"")
		{
			auto newMeshNode = std::make_shared<DxDeformerNode>();
			newMeshNode->SetName(name);
			newMeshNode->m_meshData.CreateConstBuffers(DxDeviceManager::Device());
			newMeshNode->SetDeformerNode(newMeshNode.get(), -1); // the skeleton mesh is being deformed byt THIS deformedNode

			return newMeshNode;
		}

		void LoadBindPose(std::wstring animFilePath);
		void LoadAnimClip(std::wstring animFile);

		void AttachWeapon(rldx::DxMeshNode* nodeWeapon, const std::wstring& boneName)
		{
			auto index = m_animationPlayer.GetSkeleton().GetIndexFromBoneName(libtools::wstring_to_string(boneName));

			if (index != -1)
			{
				nodeWeapon->SetDeformerNodeRecursive(this, index); // recursive, all childres are attached, too
			}
		}

		const VS_PerMeshConstBuffer_Skeleton* GetDeformerData() const;
		virtual void Update(float time) override;

	private:
		void CopyMatrices();
		// TODO: this mesh should not get get the data directly this like
		/*
			rather, renderquque should be sorted

			skeleton 1
			mesh, deformer = 1
			mesh, deformer = 1

			skeleton 2
			mesh, deformer = 2
			mesh, deformer = 2

			S_PerMeshConstBuffer_Skeleton m_constBufferDerformerData_VS; ->const buffer
		*/

		VS_PerMeshConstBuffer_Skeleton m_constBufferDerformerData_VS;
	};
}
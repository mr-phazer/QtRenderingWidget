#pragma once

#include <string>

// TODO: make as many forward declarations as possible
#include <FileFormats\Anim\Reader\TwAnimReader.h>
#include <Rldx\Animation\DataTypes\Skeleton.h>
#include <Rldx\Animation\Managers\AnimatorService\AnimContainer\AnimQueue.h>

#include "..\..\DataTypes\ConstBuffers\CPUConstBuffers.h"
#include "DxMeshNode.h"

namespace rldx
{
	// TODO: Refector: make is to there is only ONE deformer node (no derived classes, unless strictly neededed) 
	// TODO: make class as simple as possible
	class DxDeformerNode
		: public DxMeshNode // TODO: mesh node for "skeleton stick figure" should be member/base/class/wholly separete?
	{
		// TODO: remove: make private
	protected:
		skel_anim::Skeleton  m_skeleton;
		skel_anim::AnimQueue m_animQueue;
		anim_file::TwAnimFileReader m_animFileReader; // TODO: Refactor: should this really be stored here?

		std::vector<sm::Matrix> m_framePoseMatrices;

	public:
		static std::shared_ptr<DxDeformerNode> Create(const std::wstring& name = L"");

		virtual void LoadBindPose(std::wstring animFilePath);
		virtual void LoadAnimClip(std::wstring animFile);

		virtual void AttachWeapon(rldx::DxMeshNode* nodeWeapon, const std::wstring& boneName);

		const VS_PerMeshConstBuffer_Skeleton* GetDeformerData() const;
		virtual void Update(float time) override;

	private:
		virtual void CopyMatrices();


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
	protected:
		VS_PerMeshConstBuffer_Skeleton m_constBufferDerformerData_VS;
	};
}
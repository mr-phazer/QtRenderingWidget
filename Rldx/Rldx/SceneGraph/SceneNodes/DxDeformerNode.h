#pragma once

// TODO: make as many forward declarations as possible
#include <FileFormats\Anim\Reader\TwAnimReader.h>
#include "..\..\Animation\AnimationPlayer.h"
#include "..\..\Helpers\DxMeshCreatorHelper.h"
#include "..\..\Managers\DxDeviceManager.h"
#include "..\..\Rendering\DxShaderProgram.h"
#include "DxMeshNode.h"


namespace rldx
{
	class DxDeformerNode : public DxMeshNode
	{
		skel_anim::AnimationPlayer m_animationPlayer;

	public:
		//std::shared_ptr<DxDeformerNode> Create(const std::wstring& name)
		//{
		//	auto newDeformerNode = std::make_shared<DxDeformerNode>();
		//	newDeformerNode->SetName(name);
		//	return newDeformerNode;
		//}

		void LoadBindPose(std::wstring animFilePath)
		{
			auto animBytes = DxResourceManager::GetCallBackFile(animFilePath);
			auto animFile = anim_file::TwAnimFileReader().Read(animBytes);

			m_animationPlayer.CreateBindPose(animFile);

			auto skeletonMesh = DxSkeletonMeshCreator::Create(
				DxDeviceManager::Device(),
				m_animationPlayer.GetSkeleton()
			);

			SetModelData(skeletonMesh);

			auto simpleShaderProgram =
				rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
					DxDeviceManager::Device(),
					LR"(VS_Simple.cso)",
					LR"(PS_Simple.cso)"
				);

			SetShaderProgram(simpleShaderProgram);
		}


		void LoadAnimClip(std::wstring animFile);

		std::vector<sm::Matrix> GetFramePoseMatrices();

		virtual void Update(float time) override
		{

		}

		/*
		TODO: add the AnimationManager here

		void CreateBindPose(file)
		{
			- load file int AnimationManager
			- generate skeleton stick-figure
			- set shader and mesh data as for the GRID
		}
		void LoadAnimationClip(file)

		*/


	};
}
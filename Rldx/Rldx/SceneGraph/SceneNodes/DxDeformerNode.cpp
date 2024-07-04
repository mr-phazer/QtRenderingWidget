#include "DxDeformerNode.h"

#include <Rldx\Animation\DataTypes\SkeletonAnimation.h>
#include <Rldx\Animation\FramePoseGenerator.h>
#include <Rldx\Animation\Helpers\AnimationCreator.h>
#include <Rldx\Animation\Helpers\AnimationRemapper.h>
#include <Rldx\Animation\Managers\AnimatorService\AnimContainer\AnimContainer.h>
#include <Rldx\Helpers\DxMeshCreatorHelper.h>
#include "..\..\Animation\Helpers\SkeletonHelpers.h"
#include "..\..\Helpers\DxMeshCreatorHelper.h"
#include "..\..\Managers\DxDeviceManager.h"
#include "..\..\Rendering\DxShaderProgram.h"

namespace rldx
{
	std::unique_ptr<DxDeformerNode> DxDeformerNode::Create(const std::wstring& name)
	{
		auto newMeshNode = std::make_unique<DxDeformerNode>();
		newMeshNode->SetName(name);
		newMeshNode->m_meshData.CreateConstBuffers_DOES_NOTHING__REMOVE(DxDeviceManager::Device());
		newMeshNode->SetDeformerNode(newMeshNode.get(), -1); // the skeleton mesh is being deformed byt THIS deformedNode

		return newMeshNode;
	}

	void DxDeformerNode::LoadBindPose(std::wstring animFilePath)
	{
		auto animBindPoseBytes = rldx::DxResourceManager::GetFile(animFilePath);
		auto animBindPoseFile = m_animFileReader.Read(animBindPoseBytes);

		// For rome/Attila/ToB skeleton "rome_man_game" is needed to load certain "human" models / animations
		// The two skeletons are functionally identical, aside from extra bones in the hand and head				
		ForceCorrectSkeleton(m_skeleton);

		m_skeleton = skel_anim::Skeleton(animBindPoseFile);

		auto skeletonMesh = rldx::DxSkeletonMeshCreator::Create(
			rldx::DxDeviceManager::Device(),
			m_skeleton);

		SetMeshData(skeletonMesh, L"Skeleton Mesh");

		// TODO: put this into a virtual void DxBaseNode::UpdateBoundingBox(DxCommonMeshData&)
		DirectX::BoundingBox bbout;
		DirectX::BoundingBox::CreateFromPoints(
			bbout,
			skeletonMesh.originalMeshData.vertices.size(),
			(const DirectX::XMFLOAT3*)skeletonMesh.originalMeshData.vertices.data(),
			sizeof(rldx::CommonVertex));

		SetBoundingBox(bbout);

		auto simpleShaderProgram = DefaultShaderCreator::GetSimpleShaderProgram();
		SetShaderProgram(simpleShaderProgram);

		m_meshData.CreateConstBuffers_DOES_NOTHING__REMOVE(rldx::DxDeviceManager::Device());

		auto& invMatrices = m_skeleton.GetInverseBindPoseMatrices();
		memcpy(&m_constBufferDerformerData_VS.inverseBindPoseMatrices, invMatrices.data(), sizeof(sm::Matrix) * invMatrices.size());

		SetDeformerNode(this, -1);
	}

	void DxDeformerNode::LoadAnimClip(std::wstring m_animFilePath)
	{
		// creates 1 animation from the 
		skel_anim::AnimationCreator animCreateor(m_animFilePath, m_skeleton);

		auto sourceSkeletonName = animCreateor.GetSkeletonName();

		if (CompareNoCase(L"rome_man_game", sourceSkeletonName))
		{
			// Performs a remap of the m_animation to the current skeleton, if needed
			skel_anim::AnimationRemapper animRampper(sourceSkeletonName, m_skeleton.GetName());
			*animCreateor.GetAnimation() = animRampper.RemapAnimation(*animCreateor.GetAnimation());
		}

		m_animQueue.AddAnimation(animCreateor.GetAnimation());
	}


	void DxDeformerNode::AttachWeapon(rldx::DxMeshNode* nodeWeapon, const std::wstring& boneName)
	{
		auto index = m_skeleton.GetIndexFromBoneName(ToString(boneName));

		if (index != -1)
		{
			nodeWeapon->SetDeformerNode(this, index); // recursive, all childres are attached, too
		}
	}

	const VS_PerMeshConstBuffer_Skeleton* DxDeformerNode::GetDeformerData() const {

		return &m_constBufferDerformerData_VS;
	}

	void DxDeformerNode::Update(float time)
	{
		DxMeshNode::Update(time);
		auto keyFrame = m_animQueue.Sample(time);
		skel_anim::FramePoseGenerator(m_skeleton).GenerateMatrices(keyFrame, m_framePoseMatrices);

		m_constBufferDerformerData_VS.boneCount = static_cast<uint32_t>(m_framePoseMatrices.size());

		for (size_t i = 0; i < m_constBufferDerformerData_VS.boneCount; i++)
		{
			m_constBufferDerformerData_VS.boneTransform[i] = (m_skeleton.GetInverseBindPoseMatrices()[i] * m_framePoseMatrices[i]).Transpose();
		}
	}
}
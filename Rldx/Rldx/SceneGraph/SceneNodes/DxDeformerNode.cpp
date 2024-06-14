#include "DxDeformerNode.h"

#include <Rldx\Animation\DataTypes\SkeletonAnimation.h>
#include <Rldx\Animation\FramePoseGenerator.h>
#include <Rldx\Animation\Managers\AnimatorService\AnimContainer\AnimContainer.h>
#include <Rldx\Helpers\DxMeshCreatorHelper.h>
#include "..\..\Helpers\DxMeshCreatorHelper.h"
#include "..\..\Managers\DxDeviceManager.h"
#include "..\..\Rendering\DxShaderProgram.h"

namespace rldx
{
	std::shared_ptr<DxDeformerNode> DxDeformerNode::Create(const std::wstring& name)
	{
		auto newMeshNode = std::make_shared<DxDeformerNode>();
		newMeshNode->SetName(name);
		newMeshNode->m_meshData.CreateConstBuffers(DxDeviceManager::Device());
		newMeshNode->SetDeformerNode(newMeshNode.get(), -1); // the skeleton mesh is being deformed byt THIS deformedNode

		return newMeshNode;
	}

	void DxDeformerNode::LoadBindPose(std::wstring animFilePath)
	{
		auto animBindPoseBytes = rldx::DxResourceManager::GetCallBackFile(animFilePath);
		auto animBindPoseFile = m_animFileReader.Read(animBindPoseBytes);

		m_animFileReader.SetBindPose(animBindPoseFile); // TODO: maybe put this somewhere else? Inside the TWAnimFileReader class?

		m_skeleton = skel_anim::Skeleton(animBindPoseFile);

		auto skeletonMesh = rldx::DxSkeletonMeshCreator::Create(
			rldx::DxDeviceManager::Device(),
			m_skeleton);

		SetMeshData(skeletonMesh, L"Skeleton Mesh");

		DirectX::BoundingBox bbout;
		DirectX::BoundingBox::CreateFromPoints(
			bbout,
			skeletonMesh.originalMeshData.vertices.size(),
			(DirectX::XMFLOAT3*)skeletonMesh.originalMeshData.vertices.data(),
			sizeof(rldx::CommonVertex));

		SetBoundingBox(bbout);

		auto simpleShaderProgram =
			rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
				rldx::DxDeviceManager::Device(),
				LR"(VS_weighted4.cso)",
				LR"(PS_Simple.cso)"
			);

		SetShaderProgram(simpleShaderProgram);
		m_meshData.CreateConstBuffers(rldx::DxDeviceManager::Device());

		auto& invMatrices = m_skeleton.GetInverseBindPoseMatrices();
		memcpy(&m_constBufferDerformerData_VS.inverseBindPoseMatrices, invMatrices.data(), sizeof(sm::Matrix) * invMatrices.size());
	}

	void DxDeformerNode::LoadAnimClip(std::wstring animFilePath)
	{
		auto animBytes = rldx::DxResourceManager::GetCallBackFile(animFilePath);
		auto animFile = m_animFileReader.Read(animBytes);

		m_animQueue.AddAnimation(skel_anim::SkeletonAnimation::CreateFromAnimFile(animFile));
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

	void DxDeformerNode::CopyMatrices()
	{
		auto& invPoseMatrices = m_skeleton.GetInverseBindPoseMatrices();

		m_constBufferDerformerData_VS.boneCount = static_cast<uint32_t>(m_framePoseMatrices.size());

		for (size_t i = 0; i < m_framePoseMatrices.size(); i++)
		{
			m_constBufferDerformerData_VS.boneTransform[i] = (invPoseMatrices[i] * m_framePoseMatrices[i]).Transpose();
		}
	}
}
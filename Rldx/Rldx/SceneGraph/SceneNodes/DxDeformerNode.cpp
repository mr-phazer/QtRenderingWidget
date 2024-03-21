#include "DxDeformerNode.h"


namespace rldx
{

	void DxDeformerNode::LoadBindPose(std::wstring animFilePath)
	{
		auto animBindPoseBytes = DxResourceManager::GetCallBackFile(animFilePath);
		auto animBindPoseFile = m_animFileReader.Read(animBindPoseBytes);
		m_animFileReader.SetBindPose(animBindPoseFile); // TODO: maybe put this somewhere else? Inside the TWAnimFileReader class?

		m_animationPlayer.CreateBindPose(animBindPoseFile);

		auto skeletonMesh = DxSkeletonMeshCreator::Create(
			DxDeviceManager::Device(),
			m_animationPlayer.GetSkeleton()
		);

		SetMeshData(skeletonMesh, L"Skeleton Mesh");

		DirectX::BoundingBox bbout;
		DirectX::BoundingBox::CreateFromPoints(
			bbout,
			skeletonMesh.originalMeshData.vertices.size(),
			(DirectX::XMFLOAT3*)skeletonMesh.originalMeshData.vertices.data(),
			sizeof(CommonVertex));

		SetBoundingBox(bbout);

		auto simpleShaderProgram =
			DxMeshShaderProgram::Create<DxMeshShaderProgram>(
				DxDeviceManager::Device(),
				LR"(VS_weighted4.cso)",
				LR"(PS_Simple.cso)"
			);

		SetShaderProgram(simpleShaderProgram);
		m_meshData.CreateConstBuffers(DxDeviceManager::Device());

		auto& invMatrices = m_animationPlayer.GetInverseBindPoseMatrices();
		memcpy(&m_constBufferDerformerData_VS.inverseBindPoseMatrices, invMatrices.data(), sizeof(sm::Matrix) * invMatrices.size());
	}

	void DxDeformerNode::LoadAnimClip(std::wstring animFilePath)
	{
		auto animBytes = DxResourceManager::GetCallBackFile(animFilePath);
		auto animFile = m_animFileReader.Read(animBytes);

		m_animationPlayer.SetAnimation(animFile);
	}

	const VS_PerMeshConstBuffer_Skeleton* DxDeformerNode::GetDeformerData() const {

		return &m_constBufferDerformerData_VS;
	}

	void DxDeformerNode::Update(float timeElapsed)
	{
		DxMeshNode::Update(timeElapsed);
		m_animationPlayer.Update(timeElapsed);

		CopyMatrices();
	}

	void DxDeformerNode::CopyMatrices()
	{
		auto& framePoseMatrices = m_animationPlayer.GetFramPoseMatrices();
		auto& invPoseMatrices = m_animationPlayer.GetInverseBindPoseMatrices();

		m_constBufferDerformerData_VS.boneCount = static_cast<uint32_t>(framePoseMatrices.size());

		for (size_t i = 0; i < framePoseMatrices.size(); i++)
		{
			m_constBufferDerformerData_VS.boneTransform[i] = (invPoseMatrices[i] * framePoseMatrices[i]).Transpose();
		}
	}
}
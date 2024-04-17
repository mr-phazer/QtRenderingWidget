//#include "DxAnimatorServiceNode.h"
//
//#include <Rldx\Animation\DataTypes\SkeletonAnimation.h>
//#include <Rldx\Helpers\DxMeshCreatorHelper.h>
//#include <Rldx\Managers\ResourceManager\DxResourceManager.h>
//#include <Rldx\Rendering\DxShaderProgram.h>
//
//using namespace skel_anim;
//
//namespace rldx
//{
//	void DxAnimatorServiceNode::LoadBindPose(std::wstring animFilePath)
//	{
//		auto animBindPoseBytes = rldx::DxResourceManager::GetCallBackFile(animFilePath);
//		auto animBindPoseFile = m_animFileReader.Read(animBindPoseBytes);
//
//		m_animFileReader.SetBindPose(animBindPoseFile); // TODO: maybe put this somewhere else? Inside the TWAnimFileReader class?
//
//		CreateBindPose(animBindPoseFile);
//
//		auto skeletonMesh = rldx::DxSkeletonMeshCreator::Create(
//			rldx::DxDeviceManager::Device(),
//			m_skeleton);
//
//		SetMeshData(skeletonMesh, L"Skeleton Mesh");
//
//		DirectX::BoundingBox bbout;
//		DirectX::BoundingBox::CreateFromPoints(
//			bbout,
//			skeletonMesh.originalMeshData.vertices.size(),
//			(DirectX::XMFLOAT3*)skeletonMesh.originalMeshData.vertices.data(),
//			sizeof(rldx::CommonVertex));
//
//		SetBoundingBox(bbout);
//
//		auto simpleShaderProgram =
//			rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
//				rldx::DxDeviceManager::Device(),
//				LR"(VS_weighted4.cso)",
//				LR"(PS_Simple.cso)"
//			);
//
//		SetShaderProgram(simpleShaderProgram);
//		m_meshData.CreateConstBuffers(rldx::DxDeviceManager::Device());
//
//		auto& invMatrices = m_skeleton.GetInverseBindPoseMatrices();
//		memcpy(&m_constBufferDerformerData_VS.inverseBindPoseMatrices, invMatrices.data(), sizeof(sm::Matrix) * invMatrices.size());
//	}
//
//	void DxAnimatorServiceNode::LoadAnimClip(std::wstring animFilePath)
//	{
//		auto animBytes = rldx::DxResourceManager::GetCallBackFile(animFilePath);
//		auto animFile = m_animFileReader.Read(animBytes);
//
//		m_animQueue.AddAnimation(skel_anim::SkeletonAnimation::CreateFromAnimFile(animFile));
//
//		//m_animationPlayer.SetAnimation(animFile);
//	}
//
//	void DxAnimatorServiceNode::Update(float time)
//	{
//		DxMeshNode::Update(time);
//		auto keyFrame = m_animQueue.Sample(time);
//		FramePoseGenerator(m_skeleton).GenerateMatrices(keyFrame, m_framePoseMatrices);
//
//		m_constBufferDerformerData_VS.boneCount = static_cast<uint32_t>(m_framePoseMatrices.size());
//
//		for (size_t i = 0; i < m_constBufferDerformerData_VS.boneCount; i++)
//		{
//			m_constBufferDerformerData_VS.boneTransform[i] = (m_skeleton.GetInverseBindPoseMatrices()[i] * m_framePoseMatrices[i]).Transpose();
//		}
//	}
//
//
//
//	//void DxAnimatorServiceNode::LoadBindPose(const std::string& skeletonName)
//	//{
//	//	//m_deformerNode = rldx::DxDeformerNode::Create();
//	//	LoadBindPose(path_helper::GetPathFromSkeletonName(skeletonName));
//
//	//	m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_01.anim)");
//	//	m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_02.anim)");
//	//	m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_03.anim)");
//	//	m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_04.anim)");
//	//	m_deformerNode->LoadAnimClip(LR"(I:\Modding\WH3\animations\battle\humanoid01\sword_and_shield\stand\hu1_sws_stand_idle_05.anim)");
//
//
//	//	//		poSceneNode->AddChild(m_deformerNode);
//	//}
//}

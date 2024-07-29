#pragma once

#include <string>

// TODO: make as many forward declarations as possible
#include <ImportExport\FileFormats\Anim\Reader\TwAnimReader.h>
#include <Rldx\Rldx\Animation\DataTypes\Skeleton.h>
#include <Rldx\Rldx\Animation\Managers\AnimatorService\AnimContainer\AnimQueue.h>

#include "..\..\DataTypes\ConstBuffers\CPUConstBuffers.h"
#include "DxMeshNode.h"

namespace rldx
{
	class IFramePoseProvider
	{
	public:
		virtual void GetFramePoseMatrices(std::vector<sm::Matrix>& outFramePoseMatrices) = 0;
		virtual void Update(float timeElapsed) = 0;
	};

	/// <summary>
	/// TODO: make this part of DxDeformerNode
	/// TODO: this is either called from the mesh it deforms
	/// OR the renderqueue is sorted to {Derformer1, MeshDeformBy1, Derformer2, MeshDeformBy2, ANotherMeshDeformBy2}
	/// </summary>
	struct DxDerformerData : public IRenderQueueItem
	{
		std::vector<sm::Matrix>_framePoseMatrices;
	};

	// TODO: Refector: make is to there is only ONE deformer node (no derived classes, unless strictly neededed) 
	// TODO: make class as simple as possible
	class DxDeformerNode : public DxMeshNode // TODO: mesh node for "skeleton stick figure" should be member/base/class/wholly separete?
	{
		// TODO: remove: make private

	protected:
		skel_anim::Skeleton m_skeleton;
		skel_anim::AnimQueue m_animQueue;
		anim_file::TwAnimFileReader m_animFileReader; // TODO: Refactor: should this really be stored here?

		// TODO: no neead to copy into this first, just copy directly to VS_PerMeshConstBuffer_Skeleton
		std::vector<sm::Matrix> m_framePoseMatrices;
		VS_PerMeshConstBuffer_Skeleton m_constBufferDerformerData_VS;

	public:
		DxDeformerNode()
		{
			SetType(SceneNodeTypeEnum::BaseNode);
			SetTypeString(L"Node:DxDeformerNode");
		}

		DxDeformerNode(const std::wstring& name)
		{
			DxDeformerNode();
			SetName(name);
		}

		virtual ~DxDeformerNode();
		static std::unique_ptr<DxDeformerNode> Create(const std::wstring& m_nodeName = L"");

		virtual void LoadBindPose(rldx::DxResourceManager& resourceManager, std::wstring animFilePath);
		virtual void LoadAnimClip(rldx::DxResourceManager& resourceManager, std::wstring m_animFilePath);
		virtual void AttachWeapon(rldx::DxMeshNode* nodeWeapon, const std::wstring& boneName);
		virtual void Update(float time) override;

		const skel_anim::Skeleton& GetSkeleton() { return m_skeleton; }

		const VS_PerMeshConstBuffer_Skeleton* GetDeformerData() const;
	};
}
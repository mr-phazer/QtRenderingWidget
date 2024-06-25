//#pragma once
//#include <memory>
//#include <Rldx\Animation\Managers\AnimatorService\AnimContainer\AnimQueue.h>
//#include <Rldx\SceneGraph\SceneNodes\DxDeformerNode.h>
//#include <string>
//
//namespace rldx
//{
//	class DxBaseNode;
//	class DxMeshNode;
//	class DxDeformerNode;
//}
//
//namespace rldx
//{
//	// forward declarations
//	class AnimContainer;
//
//	class DxAnimatorServiceNode : public rldx::DxDeformerNode
//	{
//		skel_anim::AnimQueue m_animQueue;
//		skel_anim::Skeleton m_skeleton;
//		std::vector<sm::Matrix> m_framePoseMatrices;
//
//	public:
//		DxAnimatorServiceNode() = default;
//
//
//		void LoadBindPose(std::wstring m_animFilePath) override;
//		void LoadAnimClip(std::wstring m_animFilePath) override;
//
//		void Update(float time);
//
//
//		void CreateBindPose(const anim_file::AnimFile& animFile);
//
//
//
//
//
//		/// <summary>	
//		/// Load 
//		///  
//		/// 
//		/// </summary>	
//		/// <param name="poSceneNode">The scene node to attach the skeleton to.</param>
//
//		//void LoadBindPose(const std::string& skeletonName);
//
//
//
//	};
//
//
//
//}
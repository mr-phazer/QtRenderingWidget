#pragma once

#include "..\..\..\..\DirectXTK\Inc\SimpleMath.h"

namespace rldx {

	struct TransformState
	{
		sm::Vector3 translation = { 0,0,0 };
		sm::Vector3 scale = { 1,1,1 };
		sm::Quaternion rotation = sm::Quaternion::Identity;
	};

	/// <summary>
	/// Stores traslation, rotation and scale, 
	/// and calculates the global transform matrix from the those values
	/// </summary>
	class NodeTransform
	{
	public:
		/// <summary>
		/// Caclulates the "global" transform matrix from "global = parent X local"
		/// used recusively the "client" can get the proper global transform
		/// </summary>
		/// <param name="_m"></param>
		/// <returns></returns>
		sm::Matrix GetGlobalTransform(const sm::Matrix& _m);		
		sm::Matrix LocalTransform() const;		

		void SetTransformFromMatrix(const sm::Matrix& _mIn);
		
		void SetTranslation(const sm::Vector3 _translation);
		void SetTranslation(float x, float y, float z);

		void SetRotation(const sm::Quaternion& _quaterion);
		void SetRotationFromYawPitchRoll(const sm::Vector3& _rotation);

		void SetScale(const sm::Vector3& _scale);
		void SetScale(float _scale);
			

		sm::Vector3 GetTranslation();
		sm::Quaternion GetRotation();
		sm::Vector3 GetScale();
	


	private:
		TransformState local;		
	};
};
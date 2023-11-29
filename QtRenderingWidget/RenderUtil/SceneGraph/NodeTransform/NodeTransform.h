#pragma once

#include "..\..\..\..\DirectXTK\Inc\SimpleMath.h"

namespace sm = DirectX::SimpleMath;

namespace Rldx {


	/// <summary>
	/// Stores traslation, rotation and scale, 
	/// and calculates the global transform matrix from the those values
	/// </summary>
	class NodeTransform
	{
	public:
		/// <summary>
		/// Caclulates the "global" transform matrix from parent X local
		/// </summary>
		/// <param name="_m"></param>
		/// <returns></returns>
		sm::Matrix GetGlobalTransform(const sm::Matrix& _m);

		//struct TransformHelper

		sm::Matrix GetTransform() const;		
		void SetTransForm(const sm::Matrix& _mIn);

		// set each "type" of transform "independently"
		void SetTranslation(const sm::Vector3 _translation);
		void SetTranslation(float x, float y, float z);

		void SetRotation(const sm::Quaternion& _quaterion);
		void SetYawPitchRoll(const sm::Vector3& _rotation);

		void SetScale(const sm::Vector3& _scale);
		void SetScale(float _scale);

		sm::Vector3 GetScale();
		sm::Vector3 GetTranslation();
		sm::Quaternion GetRotation();

	private:
		struct TransformValue
		{
			sm::Vector3 translation = { 0,0,0 };
			sm::Vector3 scale = { 1,1,1 };
			sm::Quaternion rotation = sm::Quaternion::Identity;

		} local;	
	};

	//TransformHelper local;
	//TransformHelper global; // *these are only valid when the host scengraph has been traversed once	

};
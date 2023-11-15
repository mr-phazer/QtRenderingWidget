#pragma once

#include "..\..\..\..\DirectXTK\Inc\SimpleMath.h"

namespace sm = DirectX::SimpleMath;

namespace Rldx {

	class NodeTransform
	{
	public:
		/// <summary>
		/// does global = local * _m
		/// </summary>
		/// <param name="_m"></param>
		void doGlobalTransform(const sm::Matrix& _m);

		//struct TransformHelper

		sm::Matrix getLocalTransform();
		sm::Matrix& getGlobalTransformMatrix();
		void setGlobalTransformMatrix(const sm::Matrix&);

		void setTransForm(const sm::Matrix& _mIn);

		// set each "type" of transform "independently"
		void setTranslation(const sm::Vector3 _translation);
		void setTranslation(float x, float y, float z);
		void setRotation(const sm::Quaternion& _quaterion);
		void setYawPitchRoll(const sm::Vector3& _rotation);

		void setScale(const sm::Vector3& _scale);
		void setScale(float _scale);

		sm::Vector3 getScale();
		sm::Vector3 getTranslation();
		sm::Quaternion getRotation();

	private:
		struct TransformValue
		{
			sm::Vector3 translation = { 0,1,0 };
			sm::Vector3 scale = { 1,1,1 };
			sm::Quaternion rotation = sm::Quaternion::Identity;

		} local;

	private:
		sm::Matrix m_mGlobalTransformMatrix;
	};

	//TransformHelper local;
	//TransformHelper global; // *these are only valid when the host scengraph has been traversed once	

};
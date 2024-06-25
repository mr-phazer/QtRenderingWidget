#include "NodeTransform.h"

using namespace DirectX::SimpleMath;

namespace rldx
{

	const sm::Matrix NodeTransform::LocalTransform() const
	{
		auto m4x4LocalTransformMatrix =
			::sm::Matrix::CreateTranslation(m_localTranform.translation) *
			::sm::Matrix::CreateFromQuaternion(m_localTranform.rotation) *
			::sm::Matrix::CreateScale(m_localTranform.scale);

		return m4x4LocalTransformMatrix;
	}

	void NodeTransform::SetTransformFromMatrix(const sm::Matrix& mIn)
	{
		sm::Matrix tempMatrix = mIn;
		tempMatrix.Decompose(m_localTranform.scale, m_localTranform.rotation, m_localTranform.translation);
	}

	sm::Matrix NodeTransform::GetGlobalTransform(const sm::Matrix& parent) const
	{
		return parent * LocalTransform();
	}

	void NodeTransform::SetRotation(const sm::Quaternion& q)
	{
		m_localTranform.rotation = q;
	}

	void NodeTransform::SetRotationFromYawPitchRoll(const sm::Vector3& rotation)
	{
		// TODO: make a "m_localTranform.rotation = GetQuaterionFromEuler(x,y,z);"
		// this is clumsy:  ?
		auto rotMatrix = sm::Matrix::CreateFromYawPitchRoll(rotation.x, rotation.y, rotation.z);
		sm::Vector3 unusedScale, unsuedTranlation;
		rotMatrix.Decompose(unusedScale, m_localTranform.rotation, unsuedTranlation);
	}

	void NodeTransform::SetTranslation(const sm::Vector3 translation)
	{
		m_localTranform.translation = translation;
	}

	void NodeTransform::SetTranslation(float x, float y, float z)
	{
		SetTranslation(sm::Vector3(x, y, z));
	}

	void NodeTransform::SetScale(const sm::Vector3& scale)
	{
		m_localTranform.scale = scale;
	}

	void NodeTransform::SetScale(float scale)
	{
		m_localTranform.scale = { scale, scale, scale };
	}

	sm::Vector3 NodeTransform::GetScale()
	{
		return m_localTranform.scale;
	}

	sm::Vector3 NodeTransform::GetTranslation()
	{
		return m_localTranform.translation;
	}

	sm::Quaternion NodeTransform::GetRotation()
	{
		return m_localTranform.rotation;
	}
}
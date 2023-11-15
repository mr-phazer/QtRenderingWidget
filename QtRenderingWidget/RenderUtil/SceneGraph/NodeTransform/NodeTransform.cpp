#include "NodeTransform.h"

using namespace Rldx;

sm::Matrix NodeTransform::getLocalTransform()
{
	auto mLocalTransformMatrix =
		sm::Matrix::CreateTranslation(local.translation) *
		sm::Matrix::CreateFromQuaternion(local.rotation) *
		sm::Matrix::CreateScale(local.scale);
	
	return mLocalTransformMatrix;
}

sm::Matrix& NodeTransform::getGlobalTransformMatrix()
{
	return m_mGlobalTransformMatrix;
}

void NodeTransform::setGlobalTransformMatrix(const sm::Matrix& _mIn)
{
	m_mGlobalTransformMatrix = _mIn;
}

void NodeTransform::setTransForm(const sm::Matrix& _mIn)
{
	sm::Matrix m = _mIn;
	m.Decompose(local.scale, local.rotation, local.translation);

	m_mGlobalTransformMatrix = _mIn;
}

void NodeTransform::doGlobalTransform(const sm::Matrix& _parent)
{		
	getGlobalTransformMatrix() = _parent * getLocalTransform();
}

void NodeTransform::setRotation(const sm::Quaternion& _q)
{
	m_mGlobalTransformMatrix *= sm::Matrix::CreateFromQuaternion(_q);
}

void NodeTransform::setYawPitchRoll(const sm::Vector3& _rotation)
{
	m_mGlobalTransformMatrix *= sm::Matrix::CreateFromYawPitchRoll(_rotation.x, _rotation.y, _rotation.z);
}

void NodeTransform::setTranslation(const sm::Vector3 _v)
{
	local.translation = _v;
	//m_TransformMatrix *= sm::Matrix::CreateTranslation(_v);
}

void NodeTransform::setTranslation(float x, float y, float z)
{
	setTranslation({ x,y,z });
}

void NodeTransform::setScale(const sm::Vector3& _v)
{
	local.scale = _v;
	//m_TransformMatrix *= sm::Matrix::CreateScale(_v);
}

void NodeTransform::setScale(float _scale)
{
	local	.scale = { _scale, _scale, _scale };
	//m_TransformMatrix *= sm::Matrix::CreateScale(_scale);
}

sm::Vector3 NodeTransform::getScale()
{
	sm::Vector3 scale, translate;
	sm::Quaternion quat;
	m_mGlobalTransformMatrix.Decompose(scale, quat, translate);

	return scale;
}

sm::Vector3 NodeTransform::getTranslation()
{
	sm::Vector3 scale, translate;
	sm::Quaternion quat;
	m_mGlobalTransformMatrix.Decompose(scale, quat, translate);

	return translate;

}

sm::Quaternion NodeTransform::getRotation()
{
	sm::Vector3 scale, translate;
	sm::Quaternion quat;
	m_mGlobalTransformMatrix.Decompose(scale, quat, translate);

	return quat;

}

#include "NodeTransform.h"

using namespace Rldx;

sm::Matrix NodeTransform::GetTransform() const
{
	auto mLocalTransformMatrix =
		sm::Matrix::CreateTranslation(local.translation) *
		sm::Matrix::CreateFromQuaternion(local.rotation) *
		sm::Matrix::CreateScale(local.scale);

	return mLocalTransformMatrix;
}

void NodeTransform::SetTransForm(const sm::Matrix& _mIn)
{
	sm::Matrix tempMatrix = _mIn;
    tempMatrix.Decompose(local.scale, local.rotation, local.translation);	
}

sm::Matrix NodeTransform::GetGlobalTransform(const sm::Matrix& _parent)
{
	return _parent * GetTransform();
}

void NodeTransform::SetRotation(const sm::Quaternion& q)
{
	local.rotation = q;
}

void NodeTransform::SetYawPitchRoll(const sm::Vector3& rotation)
{
	// TODO: make a "local.rotation = eulerToQuaterion(x,y,z);" this is clumsy?
	auto rotMatrix = sm::Matrix::CreateFromYawPitchRoll(rotation.x, rotation.y, rotation.z);
	sm::Vector3 unusedScale, unsuedTranlation;
	rotMatrix.Decompose(unusedScale, local.rotation, unsuedTranlation);
}

void NodeTransform::SetTranslation(const sm::Vector3 translation)
{
	local.translation = translation;
}

void NodeTransform::SetTranslation(float x, float y, float z)
{
	SetTranslation(sm::Vector3(x, y, z));
}

void NodeTransform::SetScale(const sm::Vector3& scale)
{
	local.scale = scale;	
}

void NodeTransform::SetScale(float scale)
{
	local.scale = { scale, scale, scale };	
}

sm::Vector3 NodeTransform::GetScale()
{
	return local.scale;
}

sm::Vector3 NodeTransform::GetTranslation()
{
	return local.translation;
}

sm::Quaternion NodeTransform::GetRotation()
{
	return local.rotation;
}

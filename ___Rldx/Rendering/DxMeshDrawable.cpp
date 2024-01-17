#include "DxMeshDrawable.h"

#include "..\Rendering/DxMesh.h"


void Rldx::DxMeshDrawable::Draw(ID3D11DeviceContext* pDeviceContext)
{
	m_poMesh->Draw(pDeviceContext);
}

void Rldx::DxMeshDrawable::BindToDC(ID3D11DeviceContext* pDeviceContext)
{
	m_poMesh->BindToDC(pDeviceContext);
	//m_poMaterial->BindToDC(pDeviceContext); // TODO: enable when works
	m_poShaderProgram->BindToDC(pDeviceContext);
}

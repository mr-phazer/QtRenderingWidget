#include "DxMeshDrawable.h"

#include "..\Rendering/DxMesh.h"


void rldx::DxMeshDrawable::Draw(ID3D11DeviceContext* pDeviceContext)
{
	m_poMesh->Draw(pDeviceContext);
}

void rldx::DxMeshDrawable::BindToDC(ID3D11DeviceContext* pDeviceContext)
{
	m_poMesh->BindToDC(pDeviceContext);
	//m_poMaterial->BindToDC(pDeviceContext); // TODO: enable when works
	m_poShaderProgram->BindToDC(pDeviceContext);
}

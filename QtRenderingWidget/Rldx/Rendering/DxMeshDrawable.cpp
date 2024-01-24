#include "DxMeshDrawable.h"

#include "..\Rendering/DxMesh.h"


void rldx::DxMeshDrawable::Draw(ID3D11DeviceContext* pDeviceContext)
{
	m_poMesh->Draw(pDeviceContext);
}

void rldx::DxMeshDrawable::BindToDC(ID3D11DeviceContext* pDeviceContext)
{
	if (m_poMesh) m_poMesh->BindToDC(pDeviceContext);	
	if (m_poMaterial) m_poMaterial->BindToDC(pDeviceContext); 
	if (m_poShaderProgram) m_poShaderProgram->BindToDC(pDeviceContext);
}

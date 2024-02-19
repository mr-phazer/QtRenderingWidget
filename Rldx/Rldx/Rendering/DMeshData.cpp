#include "DxMeshData.h"

#include "..\Rendering/DxMesh.h"


void rldx::DxMeshData::Draw(ID3D11DeviceContext* pDeviceContext)
{
	m_poMesh->Draw(pDeviceContext);
}

void rldx::DxMeshData::BindToDC(ID3D11DeviceContext* pDeviceContext)
{
	if (m_poMesh) { m_poMesh->BindToDC(pDeviceContext); }
	if (m_poShaderProgram) { m_poShaderProgram->BindToDC(pDeviceContext); }
	if (m_poMaterial) { m_poMaterial->BindToDC(pDeviceContext); }
}
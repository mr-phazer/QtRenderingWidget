#include "..\Rendering/DxMesh.h"
#include "..\SceneGraph\SceneNodes\DxDeformerNode.h"
#include "DxMeshRenderData.h"


void rldx::DxMeshRenderData::Update(float time)
{
	// TODO: 
	/*
		if (deformernode)
		{
			- get derformer matrices,
			- send to gpu
			or if (attach)
			- set global bone matrix N as world matrix


		}

	*/
}

void rldx::DxMeshRenderData::Draw(ID3D11DeviceContext* pDeviceContext)
{
	m_poMesh->Draw(pDeviceContext);
}

void rldx::DxMeshRenderData::BindToDC(ID3D11DeviceContext* pDeviceContext)
{
	if (m_poMesh) { m_poMesh->BindToDC(pDeviceContext); }
	if (m_poShaderProgram) { m_poShaderProgram->BindToDC(pDeviceContext); }
	if (m_poMaterial) { m_poMaterial->BindToDC(pDeviceContext); }
}
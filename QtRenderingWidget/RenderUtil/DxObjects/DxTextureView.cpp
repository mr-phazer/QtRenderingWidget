#include "DxTextureView.h"
#include "..\DX11Manager\DXMananger.h"
#include "vertex_layouts.h"
#include "..\ToolsLib\lodepng.h"
#include "DXMeshBuffer.h"
#include "DXMesh/DXMeshFactoryService.h"

using namespace Rldx;

std::shared_ptr<Rldx::DxTextureView> Rldx::DxTextureView::create(Rldx::DXD3DManager* _poScene, bool _bCreateDepth, UINT _width, UINT _height)
{
	auto po = std::make_shared< Rldx::DxTextureView>();

	po->init(_poScene->getDevice(), _poScene->getDeviceContext(), _width, _height);
	po->initDepthStencilView(_poScene->getDevice(), _width, _height);
	po->resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), { (float)_width, (float)_height });

	return po;
}

//void rendutil::DxTextureView::renderQuadToTexture(rendutil::Scene* _poScene, rendutil::DxTextureView* _poTextureDest)
//{
//	ID3D11ShaderResourceView* SRV[1] = { m_cpoShaderResourceView.Get() };
//
//	_poScene->getDeviceContext()->PSSetShaderResources(42, 1, SRV);
//
//	_poScene->getDeviceContext()->OMSetRenderTargets(1, _poTextureDest->m_cpoRenderTargetView.GetAddressOf(), NULL);
//
//	renderQuad(_poScene);
//
//	ID3D11RenderTargetView* NULL_RTV[1] = { NULL };
//	_poScene->getDeviceContext()->OMSetRenderTargets(1, NULL_RTV, NULL);
//}

void Rldx::DxTextureView::renderQuad(Rldx::Scene* _poScene, float _width, float _height)
{
	//	m_oBlendState.setAsActive(_poScene);

	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
		Matrix::CreateOrthographic(2.f / _width, 2.f / _height, 0.01, 25).Transpose();

	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
	_poScene->update_VS_ConstBuffer_PerFrame();

	m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());

	ID3D11ShaderResourceView* PQuadSrv[1] = {
		m_cpoShaderResourceView.Get()
	};



	m_spoQuadMesh->setStride((UINT)sizeof(QuadVertex));

	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);

	m_spoQuadMesh->drawMesh_Bare(_poScene);

	//_poScene->getDeviceContext()->IASetVertexBuffers(0, 1,
	//	m_spoQuadMesh->m_cpoVertexBuffer.GetAddressOf(),
	//	&m_spoQuadMesh->m_stride, &m_spoQuadMesh->m_offset);

	//_poScene->getDeviceContext()->IASetIndexBuffer(
	//	//m_spoQuadMesh->indexBuffer(),
	//	m_spoQuadMesh->m_cpoIndexBuffer.Get(),
	//	m_spoQuadMesh->EIndexFormat, 0
	//);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//// Draw triangles
	/////////////////////////////////////////////////////////////////////////////////////////////
	//_poScene->getDeviceContext()->IASetPrimitiveTopology(m_spoQuadMesh->ETopology);
	//_poScene->getDeviceContext()->DrawIndexed(m_spoQuadMesh->indexCount(), 0, 0);
	/////////////////////////////////////////////////////////////////////////////////////////////

	DxTextureView::unbindShaderResourceViews(_poScene, 0, 1);
}

void Rldx::DxTextureView::renderQuadToTexture(Rldx::Scene* _poScene, const DxTextureView* _poDest, float _width, float _height)
{
	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);

	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection = Matrix::CreateOrthographic(2.f / _width, 2.f / _height, 0.01f, 25.f).Transpose();
	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
	_poScene->update_VS_ConstBuffer_PerFrame();

	m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());

	ID3D11ShaderResourceView* PQuadSrv[1] = {
		m_cpoShaderResourceView.Get()
	};

	m_spoQuadMesh->setStride((UINT)sizeof(QuadVertex));

	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);

	m_spoQuadMesh->drawMesh_Bare(_poScene);

	//_poScene->getDeviceContext()->IASetVertexBuffers(0, 1,
	//	m_spoQuadMesh->m_cpoVertexBuffer.GetAddressOf(),
	//	&m_spoQuadMesh->m_stride, &m_spoQuadMesh->m_offset);

	//_poScene->getDeviceContext()->IASetIndexBuffer(
	//	//m_spoQuadMesh->indexBuffer(),
	//	m_spoQuadMesh->m_cpoIndexBuffer.Get(),
	//	m_spoQuadMesh->EIndexFormat, 0
	//);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//// Draw triangles
	/////////////////////////////////////////////////////////////////////////////////////////////
	//_poScene->getDeviceContext()->IASetPrimitiveTopology(m_spoQuadMesh->ETopology);
	//_poScene->getDeviceContext()->DrawIndexed(m_spoQuadMesh->indexCount(), 0, 0);
	/////////////////////////////////////////////////////////////////////////////////////////////

	DxTextureView::unbindShaderResourceViews(_poScene, 0, 1);
}

void Rldx::DxTextureView::copyTo_UsingCustomShader(Rldx::Scene* _poScene, DxTextureView* _poDest, Rldx::Shader* _poShader, bool _bScaleUV, float _width, float _height)
{
	D3D11_VIEWPORT vp;
	vp.Width = m_textureWidth;
	vp.Height = m_textureHeight;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_poScene->getDeviceContext()->RSSetViewports(1, &vp);

	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);

	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
		Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
	//_poScene->m_VS_PerFrame_ConstantBuffer.screenRes = { _width, _height };
	_poScene->update_VS_ConstBuffer_PerFrame();



	resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), { m_textureWidth, m_textureHeight });

	if (_bScaleUV)
	{
		//// calcuate the UV scaling:
		//// size 2 to size 1, UV_div = 1 / 2 = 0.5
		//m_u_div = (float)_poDest->getWidth() / (float)m_width;
		//m_v_div = (float)_poDest->getWidth() / (float)m_width;

		//_poScene->m_PS_PerMesh_ConstantBuffer.decal_u = m_u_div;
		//_poScene->m_PS_PerMesh_ConstantBuffer.decal_v = m_v_div;

		_poDest->m_v_div = 1.0;
		_poDest->m_u_div = 1.0;
	}
	else
	{
		m_spoQuadMesh->PSConstBuffer.Data.decal_u = m_v_div;
		m_spoQuadMesh->PSConstBuffer.Data.decal_v = m_u_div;
	}

	m_spoQuadMesh->update_PS_ConstBuffers(_poScene->getDeviceContext());

	_poShader->setAllShadersAsActive(_poScene->getDeviceContext());

	ID3D11ShaderResourceView* PQuadSrv[1] = {
		m_cpoShaderResourceView.Get()
	};

	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)_poDest->m_textureWidth, (float)_poDest->m_textureHeight);
	m_spoQuadMesh->setStride(sizeof(QuadVertex));

	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);

	m_spoQuadMesh->drawMesh_Strip(_poScene);

	DxTextureView::unbindShaderResourceViews(_poScene, 0, 1);
	DxTextureView::unbindRenderTargetView(_poScene, 0, 1);
}

void Rldx::DxTextureView::renderTo(Rldx::Scene* _poScene, DxTextureView* _poDest, bool _bScaleUV, float _width, float _height)
{
	copyTo_UsingCustomShader(_poScene, _poDest, m_spoQuadShader.get(), _bScaleUV, _width, _height);
}

void Rldx::DxTextureView::downSample(Rldx::Scene* _poScene, DxTextureView* _poDest)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)_poDest->m_textureWidth;
	vp.Height = (float)_poDest->m_textureHeight;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_poScene->getDeviceContext()->RSSetViewports(1, &vp);

	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);

	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
		Matrix::CreateOrthographic(_poDest->m_textureWidth, _poDest->m_textureHeight, 0.01, 25).Transpose();
	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;

	_poScene->update_VS_ConstBuffer_PerFrame();
	resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), { _poDest->m_textureWidth, _poDest->m_textureHeight });

	_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
	_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
	_poScene->update_PS_ConstBuffer_PerMesh();

	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);

	_poScene->update_PS_ConstBuffer_PerMesh();

	m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());

	ID3D11ShaderResourceView* PQuadSrv[1] = {
		m_cpoShaderResourceView.Get()
	};

	m_spoQuadMesh->setStride((UINT)sizeof(QuadVertex));

	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);

	m_spoQuadMesh->drawMesh_Strip(_poScene);

	DxTextureView::unbindShaderResourceViews(_poScene, 0, 1);
	DxTextureView::unbindRenderTargetView(_poScene, 0, 1);
}


void Rldx::DxTextureView::upSample(Rldx::Scene* _poScene, DxTextureView* _poDest, Rldx::Shader* _poShader)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)_poDest->m_textureWidth;
	vp.Height = (float)_poDest->m_textureHeight;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_poScene->getDeviceContext()->RSSetViewports(1, &vp);

	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);



	//_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
	//	Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
	//_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	//_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
	//_poScene->update_VS_ConstBuffer_PerFrame();

	//resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), m_textureWidth, m_textureHeight);

	//_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
	//_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
	//_poScene->update_PS_ConstBuffer_PerMesh();

	//_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);




	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
		Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
	_poScene->update_VS_ConstBuffer_PerFrame();

	resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), { m_textureWidth, m_textureHeight });

	_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
	_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
	_poScene->update_PS_ConstBuffer_PerMesh();

	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);

	if (_poShader)
	{
		_poShader->setAllShadersAsActive(_poScene->getDeviceContext());
	}
	else
	{
		m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
	}

	ID3D11ShaderResourceView* PQuadSrv[1] = {
		m_cpoShaderResourceView.Get()
	};

	m_spoQuadMesh->setStride((UINT)sizeof(QuadVertex));

	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);

	m_spoQuadMesh->drawMesh_Strip(_poScene);

	DxTextureView::unbindShaderResourceViews(_poScene, 0, 1);
	DxTextureView::unbindRenderTargetView(_poScene, 0, 1);
}


void Rldx::DxTextureView::copyToBuffer(Scene* _poScene, DxTextureView* _poDest, Shader* _poShader)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)_poDest->m_textureWidth;
	vp.Height = (float)_poDest->m_textureHeight;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_poScene->getDeviceContext()->RSSetViewports(1, &vp);

	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);



	//_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
	//	Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
	//_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	//_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
	//_poScene->update_VS_ConstBuffer_PerFrame();

	//resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), m_textureWidth, m_textureHeight);

	//_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
	//_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
	//_poScene->update_PS_ConstBuffer_PerMesh();

	//_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);




	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
		Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
	_poScene->update_VS_ConstBuffer_PerFrame();

	resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), { m_textureWidth, m_textureHeight });

	_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
	_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
	_poScene->update_PS_ConstBuffer_PerMesh();

	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);

	if (_poShader)
	{
		_poShader->setAllShadersAsActive(_poScene->getDeviceContext());
	}
	else
	{
		m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
	}

	ID3D11ShaderResourceView* PQuadSrv[1] = {
		m_cpoShaderResourceView.Get()
	};

	m_spoQuadMesh->setStride((UINT)sizeof(QuadVertex));

	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);

	m_spoQuadMesh->drawMesh_Strip(_poScene);

	DxTextureView::unbindShaderResourceViews(_poScene, 0, 1);
	DxTextureView::unbindRenderTargetView(_poScene, 0, 1);

}

void Rldx::DxTextureView::copyToBufferPosDim(Scene* _poScene, DxTextureView* _poDest, const RectFLoat& rectF, Shader* _poShader)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)_poDest->m_textureWidth;
	vp.Height = (float)_poDest->m_textureHeight;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_poScene->getDeviceContext()->RSSetViewports(1, &vp);

	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);



	//_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
	//	Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
	//_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	//_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
	//_poScene->update_VS_ConstBuffer_PerFrame();

	//resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), m_textureWidth, m_textureHeight);

	//_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
	//_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
	//_poScene->update_PS_ConstBuffer_PerMesh();

	//_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);




	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
		Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
	_poScene->update_VS_ConstBuffer_PerFrame();

	resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), { m_textureWidth, m_textureHeight });

	_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
	_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
	_poScene->update_PS_ConstBuffer_PerMesh();

	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);

	if (_poShader)
	{
		_poShader->setAllShadersAsActive(_poScene->getDeviceContext());
	}
	else
	{
		m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
	}

	ID3D11ShaderResourceView* PQuadSrv[1] = {
		m_cpoShaderResourceView.Get()
	};

	m_spoQuadMesh->setStride((UINT)sizeof(QuadVertex));

	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);

	m_spoQuadMesh->drawMesh_Strip(_poScene);

	DxTextureView::unbindShaderResourceViews(_poScene, 0, 1);
	DxTextureView::unbindRenderTargetView(_poScene, 0, 1);



}

void Rldx::DxTextureView::upSample_Cam(Rldx::Scene* _poScene, DxTextureView* _poDest, Rldx::Shader* _poShader)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)_poDest->m_textureWidth;
	vp.Height = (float)_poDest->m_textureHeight;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_poScene->getDeviceContext()->RSSetViewports(1, &vp);

	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);

	//_poScene->m_VS_PerFrame_ConstantBuffer.mProjection = Matrix(_poScene->m_oSceneCamera.GetProjMatrix()).Transpose();
	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection = Matrix(_poScene->m_oSceneCamera.GetProjMatrix()).Transpose();
	//Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
	/*_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix(_poScene->m_oSceneCamera.GetViewMatrix()).Transpose();
	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix(_poScene->m_oSceneCamera.GetViewMatrix()).Invert().Transpose();
	_poScene->update_VS_ConstBuffer_PerFrame();
	*/	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix(_poScene->m_oSceneCamera.GetViewMatrix()).Transpose();
	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix(_poScene->m_oSceneCamera.GetViewMatrix()).Invert().Transpose();
	_poScene->update_VS_ConstBuffer_PerFrame();

	resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), { m_textureWidth, m_textureHeight });

	_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
	_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
	_poScene->update_PS_ConstBuffer_PerMesh();

	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);

	if (_poShader)
	{
		_poShader->setAllShadersAsActive(_poScene->getDeviceContext());
	}
	else
	{
		m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
	}

	ID3D11ShaderResourceView* PQuadSrv[1] = {
		m_cpoShaderResourceView.Get()
	};

	m_spoQuadMesh->setStride((UINT)sizeof(QuadVertex));

	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);

	m_spoQuadMesh->drawMesh_Strip(_poScene);

	DxTextureView::unbindShaderResourceViews(_poScene, 0, 1);
	DxTextureView::unbindRenderTargetView(_poScene, 0, 1);
}


void Rldx::DxTextureView::drawDepthBuffer(Rldx::Scene* _poScene, DxTextureView* _poDest)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)_poDest->m_textureWidth;
	vp.Height = (float)_poDest->m_textureHeight;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_poScene->getDeviceContext()->RSSetViewports(1, &vp);

	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);

	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
		Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
	_poScene->update_VS_ConstBuffer_PerFrame();

	resetQuad(_poScene->getDevice(), _poScene->getDeviceContext(), { m_textureWidth, m_textureHeight });

	_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
	_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
	_poScene->update_PS_ConstBuffer_PerMesh();

	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);

	//if (_poShader)
	//{
	//	_poShader->setShader(_poScene->getDeviceContext());
	//}
	//else
	{
		m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
	}

	ID3D11ShaderResourceView* PQuadSrv[1] = {
		m_cpoDepthShaderResourceView.Get()
		//m_cpoDepthShaderResourceView.Get()
	};

	m_spoQuadMesh->setStride((UINT)sizeof(QuadVertex));

	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);

	m_spoQuadMesh->drawMesh_Strip(_poScene);

	DxTextureView::unbindShaderResourceViews(_poScene, 0, 1);
	DxTextureView::unbindRenderTargetView(_poScene, 0, 1);
}

void Rldx::DxTextureView::resolveDepthBuffer(Rldx::Scene* _poScene)
{
	//_poScene->getDeviceContext()->ResolveSubresource
	//(
	//	m_cpoTexture.Get(),
	//	D3D11CalcSubresource(0, 0, 1),
	//	m_cpoDepthStencilTexture.Get(),
	//	D3D11CalcSubresource(0, 0, 1),
	//	DXGI_FORMAT_R32_TYPELESS
	//);
}

void Rldx::DxTextureView::unbindShaderResourceViews(Rldx::Scene* _poScene, UINT _slot, UINT _count)
{
	std::vector<ID3D11ShaderResourceView*>  vecSRV_NULL(_count, NULL);
	_poScene->getDeviceContext()->PSSetShaderResources(_slot, _count, vecSRV_NULL.data());
}

void Rldx::DxTextureView::unbindRenderTargetView(Rldx::Scene* _poScene, UINT _slot, UINT _count)
{
	vector<ID3D11RenderTargetView*> vecRTV_NULL(_count, NULL);

	UINT count = (D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT < _count) ? D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT : _count;
	_poScene->getDeviceContext()->OMSetRenderTargets(count, vecRTV_NULL.data(), NULL);
}

void Rldx::DxTextureView::setActiveDestRenderTargetAndDSV(Rldx::Scene* _poScene) const
{
	_poScene->getDeviceContext()->OMSetRenderTargets(
		1,
		m_cpoRenderTargetView.GetAddressOf(), m_poDepthStencilView.Get()
	);
}

void Rldx::DxTextureView::setActiveDestRenderTargetAndDSVNULL(Rldx::Scene* _poScene) const
{
	_poScene->getDeviceContext()->OMSetRenderTargets(
		1,
		m_cpoRenderTargetView.GetAddressOf(), NULL
	);
}

void Rldx::DxTextureView::setActiveDestRenderTargetAndDSV_RAW(Rldx::Scene* _poScene) const
{
	_poScene->getDeviceContext()->OMSetRenderTargets(
		1,
		&m_poRenderTargetView_RAW, m_pDepthStencilView_RAW_External
	);
}

void Rldx::DxTextureView::setActiveDestRenderTargetOnly_RAW(Rldx::Scene* _poScene) const
{
	_poScene->getDeviceContext()->OMSetRenderTargets(
		1,
		&m_poRenderTargetView_RAW, NULL
	);
}

void Rldx::DxTextureView::setActiveDestRenderTarget_DSV_NULL(Rldx::Scene* _poScene) const
{
	_poScene->getDeviceContext()->OMSetRenderTargets(
		1,
		m_cpoRenderTargetView.GetAddressOf(),
		NULL
	);
}

void Rldx::DxTextureView::setActiveDestRenderTarget_DSV_NULL_RAW(Rldx::Scene* _poScene) const
{
	_poScene->getDeviceContext()->OMSetRenderTargets(
		1,
		&m_poRenderTargetView_RAW, NULL
	);
}

void Rldx::DxTextureView::setActiveDSVOnly_RAW(Rldx::Scene* _poScene)
{
	_poScene->getDeviceContext()->OMSetRenderTargets(
		1,
		NULL, m_pDepthStencilView_RAW_External
	);
}

void Rldx::DxTextureView::setActiveDSVOnly(Rldx::Scene* _poScene)
{
	_poScene->getDeviceContext()->OMSetRenderTargets(
		0,
		NULL, m_poDepthStencilView.Get()
	);
}

/*void rendutil::RenderTarget2D::setActiveDestRenderTarget_RAW(rendutil::Scene* _poScene)
{
	_poScene->getDeviceContext()->OMSetRenderTargets(
		1,
		&m_cpoRenderTargetView, m_poDepthStencilView
	);
}*/

void Rldx::DxTextureView::clearPixelsAndDepth(Rldx::Scene* _poScene, SimpleMath::Color vColor)
{
	_poScene->getDeviceContext()->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);

	_poScene->getDeviceContext()->ClearDepthStencilView(
		m_poDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void Rldx::DxTextureView::clearDepthOnly(Rldx::Scene* _poScene)
{
	_poScene->getDeviceContext()->ClearDepthStencilView(
		m_poDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void Rldx::DxTextureView::clearAndDepth_RAW(Rldx::Scene* _poScene, SimpleMath::Color vColor)
{
	//	_poScene->getDeviceContext()->ClearRenderTargetView(m_poRenderTargetView_RAW, vColor);
	_poScene->getDeviceContext()->ClearRenderTargetView(_poScene->m_poScreenBuffer->getRenderTargetView(), vColor);

	_poScene->getDeviceContext()->ClearDepthStencilView(
		m_pDepthStencilView_RAW_External,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void Rldx::DxTextureView::clearDepthOnly_RAW(Rldx::Scene* _poScene)
{
	_poScene->getDeviceContext()->ClearDepthStencilView(
		m_pDepthStencilView_RAW_External,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

/// <summary>
/// Operates on non ComPtr owner pointers, clearPixels framebuffer and depth buffer/stencil
/// </summary>
/// <param name="_poScene">Scene ptr</param>
/// <param name="vColor">RGBA float</param>
//void rendutil::RenderTarget2D::clearAndDepth_RAW(rendutil::Scene* _poScene, SimpleMath::Color vColor)
//{
//	//#if defined(_DEBUG)
//	//	char szDepth_Stencil_View[] = "DEPTH Stencil View";
//	//	m_poDepthStencilView_RAW->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szDepth_Stencil_View) - 1, szDepth_Stencil_View);
//	//
//	//	char szDepth_SRV_NAME[] = "DEPTH Shader Resource View";
//	//	m_poDepthShaderResourceView_RAW->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szDepth_SRV_NAME) - 1, szDepth_SRV_NAME);
//	//#endif
//
//	_poScene->getDeviceContext()->ClearRenderTargetView(m_poRenderTargetView_RAW, vColor);
//
//	_poScene->getDeviceContext()->ClearDepthStencilView(
//		m_poDepthStencilView,
//		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
//		1.0f,
//		0);
//}

void Rldx::DxTextureView::clearPixels(Rldx::Scene* _poScene, SimpleMath::Color vColor)
{
	_poScene->getDeviceContext()->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);
}

void Rldx::DxTextureView::clearPixels(ID3D11DeviceContext* _pDC, SimpleMath::Color vColor)
{
	_pDC->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);
}

void Rldx::DxTextureView::setRenderTargetView_RAW(ID3D11RenderTargetView* _poRenderTargetView)
{
	m_poRenderTargetView_RAW = _poRenderTargetView;
}

void Rldx::DxTextureView::setShaderResourceView_RAW(ID3D11ShaderResourceView* _poShaderResourceView)
{
	m_pDepthShaderResourceView_RAW_External = _poShaderResourceView;
}

void Rldx::DxTextureView::setDepthStencilView_RAW(ID3D11DepthStencilView* _poDepthStenticlView)
{
	m_pDepthStencilView_RAW_External = _poDepthStenticlView;
}

void Rldx::DxTextureView::init_views(
	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& _cpoRTV,
	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& _cpoDepthView,
	const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthTexture,
	const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthSRV,
	const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthView_ReadOnly
)
{
	if (_cpoRTV)
	{
		m_cpoRenderTargetView.Detach();
		m_cpoRenderTargetView.Attach(_cpoRTV.Get());
	}

	if (_cpoRTV)
	{
		m_poDepthStencilView.Detach();
		m_poDepthStencilView.Attach(_cpoDepthView.Get());
	}
}

bool Rldx::DxTextureView::init_ms(Rldx::Scene* _poScene, UINT _width, UINT _height, const char* _szObjectName)
{
	HRESULT hr = S_OK;

	m_textureWidth = _width;
	m_textureHeight = _height;

	///////////////////////// Map's Texture
	// Initialize the  texture description.
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));

	// Setup the texture description.
	// We will have our map be a square
	// We will need to have this texture bound as a renderQuad target AND a shader resource

	// get client dimension = renderQuad target dimensions

	/*RECT rc = getClientRect();
	UINT _width = rc.right;
	UINT _height = rc.bottom;*/

	//_poScene->getRT

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = _width;
	textureDesc.Height = _height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;

	//textureDesc.SampleDesc = m_oSampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the texture
	hr = _poScene->getDevice()->CreateTexture2D(&textureDesc, NULL, &m_cpoTexture);
	assert(SUCCEEDED(hr));

	/////////////////////// Map's Render Target
	// Setup the description of the renderQuad target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;;// D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the renderQuad target view.
	m_cpoRenderTargetView.Detach();
	hr = _poScene->getDevice()->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
	assert(SUCCEEDED(hr));

	//CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView;

	//D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMS;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.

	hr = _poScene->getDevice()->CreateShaderResourceView(m_cpoTexture.Get(),
														 &shaderResourceViewDesc, &m_cpoShaderResourceView);
	assert(SUCCEEDED(hr));

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}


bool Rldx::DxTextureView::reset(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, UINT _width, UINT _height)
{
	return _initTextureBuffers(
		_poDevice,

		_width, _height,
		m_textureDesc.Format,
		m_textureDesc.SampleDesc.Count,
		""
	);

	resetQuad(_poDevice, _poDeviceContext, { (float)_width, (float)_height });
}


bool Rldx::DxTextureView::init(
	ID3D11Device* _poDevice,
	ID3D11DeviceContext* _poDeviceContext,
	UINT _width, UINT _height,
	DXGI_FORMAT _Format,
	UINT uiSampleCount,
	const char* _szObjectName
)
{
	if (_poDeviceContext)
	{
		resetQuad(_poDevice, _poDeviceContext, { (float)_width, (float)_height });
	}

	return _initTextureBuffers(
		_poDevice,
		_width, _height,
		_Format,
		uiSampleCount,
		_szObjectName
	);
}

bool Rldx::DxTextureView::_initTextureBuffers(
	ID3D11Device* _poDevice,
	UINT _width, UINT _height,
	DXGI_FORMAT _Format,
	UINT uiSampleCount,
	const char* _szObjectName
)
{
	HRESULT hr = S_OK;

	m_textureWidth = _width;
	m_textureHeight = _height;

	///////////////////////// Map's Texture
	// Initialize the  texture description.



	ZeroMemory(&m_textureDesc, sizeof(m_textureDesc));
	ZeroMemory(&m_shaderResourceViewDesc, sizeof(m_shaderResourceViewDesc));

	// Setup the texture description.
	// We will have our map be a square
	// We will need to have this texture bound as a renderQuad target AND a shader resource

	// get client dimension = renderQuad target dimensions

	/*RECT rc = getClientRect();
	UINT _width = rc.right;
	UINT _height = rc.bottom;*/

	//_poScene->getRT

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//D3D11_TEXTURE2D_DESC textureDesc;
	m_textureDesc.Width = _width;
	m_textureDesc.Height = _height;
	m_textureDesc.MipLevels = 1;
	m_textureDesc.ArraySize = 1;
	m_textureDesc.Format = _Format;

	m_textureDesc.SampleDesc.Count = uiSampleCount;
	m_textureDesc.SampleDesc.Quality = 0;

	//textureDesc.SampleDesc = m_oSampleDesc;
	m_textureDesc.Usage = D3D11_USAGE_DEFAULT;

	if (_Format == DXGI_FORMAT_R32_TYPELESS)
		m_textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	else
		m_textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	m_textureDesc.CPUAccessFlags = 0;
	m_textureDesc.MiscFlags = 0;

	// Create the texture
	hr = _poDevice->CreateTexture2D(&m_textureDesc, NULL, m_cpoTexture.GetAddressOf());
	assert(SUCCEEDED(hr));

#if defined (DEBUG) || defined(_DEBUG)
	string strName = "Texture32";
	//m_cpoTexture->SetPrivateData(WKPDID_D3DDebugObjectName, strName.size(), strName.data());
#endif

	/////////////////////// Map's Render Target
	// Setup the description of the renderQuad target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = m_textureDesc.Format;
	renderTargetViewDesc.ViewDimension =
		(m_textureDesc.SampleDesc.Count > 1) ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the renderQuad target view.
	hr = _poDevice->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
	assert(SUCCEEDED(hr));

#if defined (DEBUG) || defined(_DEBUG)
	strName = "RTV";
	//m_cpoRenderTargetView->SetPrivateData(WKPDID_D3DDebugObjectName, strName.size(), strName.data());
#endif

	//CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView;

	//D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	m_shaderResourceViewDesc.Format = m_textureDesc.Format;
	m_shaderResourceViewDesc.ViewDimension =
		(m_textureDesc.SampleDesc.Count > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
	m_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	m_shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.

	hr = _poDevice->CreateShaderResourceView(m_cpoTexture.Get(),
											 &m_shaderResourceViewDesc, &m_cpoShaderResourceView);
	assert(SUCCEEDED(hr));

#if defined (DEBUG) || defined(_DEBUG)
	strName = "SRV";
	//m_cpoShaderResourceView->SetPrivateData(WKPDID_D3DDebugObjectName, strName.size(), strName.data());
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

bool Rldx::DxTextureView::initDepthStencilView(
	ID3D11Device* _poDevice,
	UINT _width, UINT _height,
	DXGI_FORMAT Format,
	UINT _uiSampleCount,
	const char* _szObjectName)
{
	m_textureWidth = _width;
	m_textureHeight = _height;;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Create Depth buffer TEXTURE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = _width;
	depthTextureDesc.Height = _height;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;

	depthTextureDesc.SampleDesc = { _uiSampleCount, 0 };
	depthTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;//  DXGI_FORMAT_R24G8_TYPELESS; //DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthTextureDesc.SampleDesc.Count = _uiSampleCount;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = _poDevice->CreateTexture2D(&depthTextureDesc, NULL,
									m_cpoDepthStencilTexture.GetAddressOf());
	assert(SUCCEEDED(hr));

	//string szDepthTextureName = string("DepthTeX_") + std::to_string(sm_NextId);
	/*m_cpoDepthStencilTexture->SetPrivateData(WKPDID_D3DDebugObjectName,
											 szDepthTextureName.size() - 1, szDepthTextureName.data());*/
										 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

										 /////////////////////////////////////////////////////////////////////////////////////////////////////////
										 // Create Depth  SHADER RESOURCE VIEW
										 /////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT; //DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension =
		(_uiSampleCount > 1) ?
		D3D11_SRV_DIMENSION_TEXTURE2DMS :
		D3D11_SRV_DIMENSION_TEXTURE2D;

	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	hr = _poDevice->CreateShaderResourceView(m_cpoDepthStencilTexture.Get(),
											 &shaderResourceViewDesc, m_cpoDepthShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Create Depth Stencil VIEW
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	::ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension =
		(_uiSampleCount > 1) ?
		D3D11_DSV_DIMENSION_TEXTURE2DMS :
		D3D11_DSV_DIMENSION_TEXTURE2D;

	dsvDesc.Texture2DArray.ArraySize = 1;
	dsvDesc.Texture2DArray.FirstArraySlice = 0;
	dsvDesc.Texture2DArray.MipSlice = 0;

	hr = _poDevice->CreateDepthStencilView(m_cpoDepthStencilTexture.Get(), &dsvDesc,
										   m_poDepthStencilView.GetAddressOf());
	assert(SUCCEEDED(hr));

	//string szDepthStencilViewName = string("DSV_") + std::to_string(sm_NextId);
	//m_poDepthStencilView->SetPrivateData(WKPDID_D3DDebugObjectName,
	//									 szDepthStencilViewName.size() - 1, szDepthStencilViewName.data());

									 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									 // Create READ ONLY Depth Stencil VIEW
									 ///////////////////////////////////////////////////////////////////////////////////////////////////////////
	dsvDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // DXGI_FORMAT_D24_UNORM_S8_UINT;
	hr = _poDevice->CreateDepthStencilView(m_cpoDepthStencilTexture.Get(), &dsvDesc,
										   m_cpoDepthStencilView_ReadOnly.GetAddressOf());
	assert(SUCCEEDED(hr));

	/************************
	* 	Debug naming
	************************/
	//string szDepthStencilViewNameReadOny = "DSV_RO_" + std::to_string(sm_NextId);
	//m_cpoDepthStencilView_ReadOnly->SetPrivateData(WKPDID_D3DDebugObjectName,
	//											   szDepthStencilViewNameReadOny.size() - 1, szDepthStencilViewNameReadOny.data());

											   ///*=============================================================
											   //	Create Stencil
											   //=============================================================*/
											   //D3D11_DEPTH_STENCIL_DESC dsDesc;
											   //// Depth initLog parameters
											   //dsDesc.DepthEnable = true;
											   //dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
											   //dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

											   //// Stencil initLog parameters
											   //dsDesc.StencilEnable = false;
											   //dsDesc.StencilReadMask = 0x00;
											   //dsDesc.StencilWriteMask = 0x00;

											   //// Stencil operations if pixel is front-facing
											   //dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
											   //dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
											   //dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
											   //dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

											   //// Stencil operations if pixel is back-facing
											   //dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
											   //dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
											   //dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
											   //dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

											   //// Create depth stencil state
											   //ID3D11DepthStencilState* pDSState;
											   //hr = _poScene->getDevice()->CreateDepthStencilState(&dsDesc, &m_poDepthStencilState_On);

											   //dsDesc.DepthEnable = false;
											   //hr = _poScene->getDevice()->CreateDepthStencilState(&dsDesc, &m_poDepthStencilState_Off);

											   //assert(SUCCEEDED(hr));

											   //_poScene->getDeviceContext()->OMSetDepthStencilState(m_poDepthStencilState_On, 1);
											   //m_width = _width;
											   //m_height = _height;

	return SUCCEEDED(hr);
}

//bool rendutil::RenderTarget2D::initWithFrameBuffer(rendutil::Scene* _poScene, UINT _width, UINT _height)
//{
//	//m_cpoRenderTargetView = _poScene->m_cpoRenderTargetView;
//	//m_cpoDepthStencilView = _poScene->m_poDepthStencilView;
//
//	m_poRenderTargetView_RAW = _poScene->m_cpoRenderTargetView.Get();
//	m_poDepthStencilView = _poScene->m_poDepthStencilView.Get();
//
//	return true;
//}

bool Rldx::DxTextureView::init_ms(Rldx::Scene* _poScene, UINT Width, UINT Height, DXGI_FORMAT Format, DXGI_SAMPLE_DESC SampleDesc, D3D11_USAGE Usage, UINT BindFlags)
{
	return
		SUCCEEDED(
			init_impl(
				_poScene,
				Width,
				Height,
				1,
				1,
				Format,
				SampleDesc,
				Usage,
				BindFlags,
				0,
				0
			)
		);
}

//bool rendutil::RenderTarget2D::init(rendutil::Scene* _poScene, UINT Width, UINT Height, DXGI_FORMAT Format, DXGI_SAMPLE_DESC SampleDesc, D3D11_USAGE Usage, UINT BindFlags)
//{
//	HRESULT hr = S_OK;
//
//	D3D11_TEXTURE2D_DESC textureDesc;
//	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//
//	///////////////////////// Map's Texture
//	// Initialize the  texture description.
//	ZeroMemory(&textureDesc, sizeof(textureDesc));
//	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
//
//	// Setup the texture description.
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	textureDesc.Width = Width;
//	textureDesc.Height = Height;
//	textureDesc.MipLevels = 1;
//	textureDesc.ArraySize = 1
//		textureDesc.Format = Format;
//
//	textureDesc.SampleDesc.Count = SampleDesc.Count;
//	textureDesc.SampleDesc.Quality = SampleDesc.Quality;
//
//	//textureDesc.SampleDesc = m_oSampleDesc;
//	textureDesc.Usage = Usage;
//	textureDesc.BindFlags = BindFlags;
//	textureDesc.CPUAccessFlags = 0;
//	textureDesc.MiscFlags = 0;
//
//	// Create the texture
//	hr = _poScene->getDevice()->CreateTexture2D(&textureDesc, NULL, &m_cpoTexture);
//	assert(SUCCEEDED(hr));
//
//	/////////////////////// Map's Render Target
//	// Setup the description of the renderQuad target view.
//	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
//	renderTargetViewDesc.Format = textureDesc.Format;
//	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;;// D3D11_RTV_DIMENSION_TEXTURE2D;
//	renderTargetViewDesc.Texture2D.MipSlice = 0;
//
//	// Create the renderQuad target view.
//	hr = _poScene->getDevice()->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
//	assert(SUCCEEDED(hr));
//
//	CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView;
//	shaderResourceViewDesc.Format = textureDesc.Format;
//	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
//	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
//	shaderResourceViewDesc.Texture2D.MipLevels = MipLevels;
//
//	// Create the shader resource view.
//
//	hr = _poScene->getDevice()->CreateShaderResourceView(m_cpoTexture.Get(),
//		&shaderResourceViewDesc, &m_cpoShaderResourceView);
//	assert(SUCCEEDED(hr));
//}

HRESULT Rldx::DxTextureView::init_impl(Rldx::Scene* _poScene, UINT Width, UINT Height, UINT MipLevels, UINT ArraySize, DXGI_FORMAT Format, DXGI_SAMPLE_DESC SampleDesc, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags)
{
	m_textureWidth = Width;
	m_textureHeight = Height;

	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));

	// Setup the texture description.

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	textureDesc.Width = Width;
	textureDesc.Height = Height;
	textureDesc.MipLevels = MipLevels;
	textureDesc.ArraySize = ArraySize;
	textureDesc.Format = Format;

	textureDesc.SampleDesc.Count = SampleDesc.Count;
	textureDesc.SampleDesc.Quality = SampleDesc.Quality;

	//textureDesc.SampleDesc = m_oSampleDesc;
	textureDesc.Usage = Usage;
	textureDesc.BindFlags = BindFlags;
	textureDesc.CPUAccessFlags = CPUAccessFlags;
	textureDesc.MiscFlags = MiscFlags;

	// Create the texture
	hr = _poScene->getDevice()->CreateTexture2D(&textureDesc, NULL, &m_cpoTexture);
	assert(SUCCEEDED(hr));

	/////////////////////// Map's Render Target
	// Setup the description of the renderQuad target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;;// D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the renderQuad target view.
	hr = _poScene->getDevice()->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
	assert(SUCCEEDED(hr));

	CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = MipLevels;

	// Create the shader resource view.

	hr = _poScene->getDevice()->CreateShaderResourceView(m_cpoTexture.Get(),
														 &shaderResourceViewDesc, &m_cpoShaderResourceView);
	assert(SUCCEEDED(hr));

	return hr;
}

bool Rldx::DxTextureView::createQuad(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext)
{
	if (m_spoQuadMesh)
		return true;

	m_oBlendState.create(_poDevice);

	m_spoQuadMesh = DXMeshBufferFactory::create(_poDevice);
	m_spoQuadMesh->createVertexBuffer(_poDevice, 4 * sizeof(QuadVertex), nullptr,
									  D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE
	);

	m_spoQuadMesh->createIndexBuffer(_poDevice, 4 * sizeof(uint16_t), nullptr,
									 D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE
	);

	m_spoQuadMesh->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	m_spoQuadMesh->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	m_spoQuadMesh->setIndexCount(4);

	m_spoQuadShader = Rldx::Shader::create(
		_poDevice,
		layout_Quad,
		_countof(layout_Quad),
		LR"(vS_Quad.cso)",
		LR"(ps_Quad.cso)");

	return (m_spoQuadShader && m_spoQuadMesh) ? true : false;
}

bool Rldx::DxTextureView::updateQuad(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, const RectFLoat& rectF)
{
	// set the quad to the new size
	m_quadWidth = rectF.width();
	m_quadHeight = rectF.height();

	// make 4 vertices with center in the middle 
	float halfWidth = m_quadWidth / 2;
	float halfHeight = m_quadHeight / 2;

	vector<QuadVertex> vecQuadVertices(4);
	vecQuadVertices[0].position = { -halfWidth, -halfHeight, 0.0f, 0.0f };
	vecQuadVertices[0].uv1 = { 0.0f, 1.0f };
	//vecQuad_Vertices[0].color = { 1, 0, 0, 1 };

	vecQuadVertices[1].position = { -halfWidth, halfHeight, 0.0f, 0.0f };
	vecQuadVertices[1].uv1 = { 0.0f, 0.0f };
	//vecQuad_Vertices[1].color = { 0, 0, 1, 1 };

	vecQuadVertices[2].position = { halfWidth, -halfHeight, 0.0f, 0.0f };
	vecQuadVertices[2].uv1 = { 1.0f, 1.0f };
	//vecQuad_Vertices[2].color = { 0, 1, 0, 1 };

	vecQuadVertices[3].position = { halfWidth, halfHeight, 0.0f, 0.0f };
	vecQuadVertices[3].uv1 = { 1.0f, 0.0f };
	//vecQuad_Vertices[4].color = { 0, 1, 0, 1 };

	// Optimization! 4 indices = 2 triangles, when dran as a TOPOLOGY_TRIANGLESTRIP, compated to 6 indices when using TOPOLOGY_TRIANGLELIST
	//vector<uint16_t> vecQuadIndicesTriangleStrip = {0,1,2,3}; 
	vector<uint16_t> vecQuadIndicesTriangleStrip = { 0,1,1,1 }; // TODO: remove intentional error

	// -- Copy VERTEX data from the CPU to the GPU
	D3D11_MAPPED_SUBRESOURCE pMappedResourceVertexBuffer;
	_poDeviceContext->Map(m_spoQuadMesh->m_cpoVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pMappedResourceVertexBuffer);
	std::memcpy(pMappedResourceVertexBuffer.pData, vecQuadVertices.data(), vecQuadVertices.size() * sizeof(QuadVertex));
	_poDeviceContext->Unmap(m_spoQuadMesh->m_cpoVertexBuffer.Get(), 0);

	// -- Copy index data from the CPU to the GPU
	D3D11_MAPPED_SUBRESOURCE pMappedResourceIndexBuffer;
	_poDeviceContext->Map(m_spoQuadMesh->m_cpoIndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pMappedResourceIndexBuffer);
	std::memcpy(pMappedResourceIndexBuffer.pData, vecQuadIndicesTriangleStrip.data(), vecQuadIndicesTriangleStrip.size() * 2);
	_poDeviceContext->Unmap(m_spoQuadMesh->m_cpoIndexBuffer.Get(), 0);

	return true;
}



bool Rldx::DxTextureView::resetQuad(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, const RectFLoat& rectF)
{
	createQuad(_poDevice, _poDeviceContext);
	updateQuad(_poDevice, _poDeviceContext, rectF);

	return true;


	m_quadWidth = rectF.width();
	m_quadHeight = rectF.height();

	m_oBlendState.create(_poDevice);
	// **********************************************************************************
		// quad half size
		// **********************************************************************************

	// only create new once
	if (!m_spoQuadMesh)
	{
		m_spoQuadMesh = DXMeshBufferFactory::create(_poDevice);
	}

	float left = -1.f / 2.0f;
	float right = 1.f / 2.0f;
	float top = 1.f / 2.0f;
	float bottom = -1.f / 2.0f;

	float w = m_quadWidth / 2;
	float h = m_quadHeight / 2;
	// Load the vertex array with data. a Quad, two triangles
	//vector<QuadVertex> vecQuad_Vertices(6);
	//vecQuad_Vertices[0].position = { -w, -h, 0.0f };
	//vecQuad_Vertices[0].uv1 = { 0.0f, 1.0f };
	////vecQuad_Vertices[0].color = { 1, 0, 0, 1 };

	//vecQuad_Vertices[1].position = { -w, h, 0.0f };
	//vecQuad_Vertices[1].uv1 = { 0.0f, 0.0f };
	////vecQuad_Vertices[1].color = { 0, 0, 1, 1 };

	//vecQuad_Vertices[2].position = { w, -h, 0.0f };
	//vecQuad_Vertices[2].uv1 = { 1.0f, 1.0f };
	////vecQuad_Vertices[2].color = { 0, 1, 0, 1 };

	//vecQuad_Vertices[3].position = { -w, h, 0.0f };
	//vecQuad_Vertices[3].uv1 = { 0.0f, 0.0f };
	////vecQuad_Vertices[3].color = { 1, 0, 1, 1 };

	//vecQuad_Vertices[4].position = { w, h, 0.0f };
	//vecQuad_Vertices[4].uv1 = { 1.0f, 0.0f };
	////vecQuad_Vertices[4].color = { 0, 1, 0, 1 };

	//vecQuad_Vertices[5].position = { w, -h, 0.0f };
	//vecQuad_Vertices[5].uv1 = { 1.0f, 1.0f };
	////vecQuad_Vertices[5].color = { 0, 0, 1, 1 };

	//vector<QuadVertex> vecQuad_Vertices_triangle_strip(3);
	//vecQuad_Vertices[0].position = { -w, -h, 0.0f };
	//vecQuad_Vertices[0].uv1 = { 0.0f, 1.0f };
	////vecQuad_Vertices[0].color = { 1, 0, 0, 1 };

	//vecQuad_Vertices[1].position = { -w, h, 0.0f };
	//vecQuad_Vertices[1].uv1 = { 0.0f, 0.0f };
	////vecQuad_Vertices[1].color = { 0, 0, 1, 1 };

	//vecQuad_Vertices[2].position = { w, -h, 0.0f };
	//vecQuad_Vertices[2].uv1 = { 1.0f, 1.0f };
	////vecQuad_Vertices[2].color = { 0, 1, 0, 1 };

	m_spoQuadMesh->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	m_spoQuadMesh->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	vector<QuadVertex> vecQuad_Vertices(4);
	vecQuad_Vertices[0].position = { -w, -h, 0.0f, 0.0f };
	vecQuad_Vertices[0].uv1 = { 0.0f, 1.0f };
	//vecQuad_Vertices[0].color = { 1, 0, 0, 1 };

	vecQuad_Vertices[1].position = { -w, h, 0.0f, 0.0f };
	vecQuad_Vertices[1].uv1 = { 0.0f, 0.0f };
	//vecQuad_Vertices[1].color = { 0, 0, 1, 1 };

	vecQuad_Vertices[2].position = { w, -h, 0.0f, 0.0f };
	vecQuad_Vertices[2].uv1 = { 1.0f, 1.0f };
	//vecQuad_Vertices[2].color = { 0, 1, 0, 1 };

	vecQuad_Vertices[3].position = { w, h, 0.0f, 0.0f };
	vecQuad_Vertices[3].uv1 = { 1.0f, 0.0f };
	//vecQuad_Vertices[4].color = { 0, 1, 0, 1 };

	;
	//vecIndex = {
	//	0,2,1,
	//	3,2,0
	//};
	vector<uint16_t> vecIndex = {
		0,1,2,3
	};

	vector<uint16_t> vecIndex_TriangleStrip = {
		0,2,1,
		3,2,0
	};

	m_spoQuadMesh->createVertexBuffer(_poDevice, sizeof(QuadVertex) * 6, nullptr,
									  D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE
	);

	//uint32_t indices[6];
	//for (uint32_t i = 0; i < 6; i++)
	//	indices[i] = i;

	m_spoQuadMesh->createIndexBuffer(_poDevice, vecIndex.size() * 2, nullptr,
									 D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE
	);

	// fill index buffer from CPU
	D3D11_MAPPED_SUBRESOURCE pMappedResourceIndexBuffer;
	_poDeviceContext->Map(m_spoQuadMesh->m_cpoIndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pMappedResourceIndexBuffer);
	::memcpy(pMappedResourceIndexBuffer.pData, vecIndex.data(), vecIndex.size() * 2);
	_poDeviceContext->Unmap(m_spoQuadMesh->m_cpoIndexBuffer.Get(), 0);

	// fill vertex buffer from CPU
	D3D11_MAPPED_SUBRESOURCE pMappedResourceVertexBuffer;
	_poDeviceContext->Map(m_spoQuadMesh->m_cpoVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pMappedResourceVertexBuffer);
	::memcpy(pMappedResourceVertexBuffer.pData, vecQuad_Vertices.data(), vecQuad_Vertices.size() * sizeof(QuadVertex));
	_poDeviceContext->Unmap(m_spoQuadMesh->m_cpoVertexBuffer.Get(), 0);

	m_spoQuadMesh->setIndexCount(vecIndex.size());

	/*======================================
	* Create Shader
	===========================================*/
	if (!m_spoQuadShader)
	{
		m_spoQuadShader
			= Rldx::Shader::create(
				_poDevice,
				layout_Quad,
				_countof(layout_Quad),
				LR"(vS_Quad.cso)",
				LR"(ps_Quad.cso)"
			);

		return (m_spoQuadShader) ? true : false;
	}

	return true;
};

size_t  Rldx::DxTextureView::sm_ObjectCounter = 0;
size_t  Rldx::DxTextureView::sm_NextId = 0;

//void rendutil::G_Buffer::setasRenderTargets(rendutil::Scene* _poScene)
//
//{
//	ID3D11RenderTargetView* pp_RTV_G_Buffer[2] =
//	{
//		spo_Position_Buffer->m_cpoRenderTargetView.Get(),
//		spo_Normal_Buffer->m_cpoRenderTargetView.Get()
//	};
//
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		2,
//		pp_RTV_G_Buffer,
//		_poScene->m_poDepthStencilView
//	);
//};

inline DirectX::XMFLOAT4 Rldx::DxTextureView::getPixelFromDepthBuffer(ID3D11DeviceContext* _poDeviceContext)
{
	//desc.SampleDesc.Count = 1;
	//desc.SampleDesc.Quality = 0;
	//desc.BindFlags = 0;
	//desc.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;
	//desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	//desc.Usage = D3D11_USAGE_STAGING;


	//hr = d3dDevice->CreateTexture2D(&desc, nullptr, &pStaging);
	//if (FAILED(hr))
	//	return hr;


	//D3D11_BOX srcBox;
	//srcBox.left = 10;
	//srcBox.right = srcBox.left + 1;
	//srcBox.top = 10;
	//srcBox.bottom = srcBox.top + 1;
	//srcBox.front = 0;
	//srcBox.back = 1;

	//_poDeviceContext->CopySubresourceRegion(p1x1StagingTexture, 0, 0, 0, 0, m_cpoDepthStencilTexture.Get(), 0, &srcBox);

	//D3D11_MAPPED_SUBRESOURCE msr;
	//_poDeviceContext->Map(p1x1StagingTexture, 0, D3D11_MAP_READ, 0, &msr);
	//BYTE *pixel = msr.pData;
	//// copy data
	//_poDeviceContexts->Unmap(p1x1StagingTexture, 0);
}

bool Rldx::DxTextureView::saveToDisk(ID3D11DeviceContext* _poDeviceContext, const string& _strFileName)
{
	ID3D11Texture2D* pBackBuffer = this->m_cpoTexture.Get();
	ID3D11Texture2D* pStagingBackBuffer = nullptr;

	D3D11_TEXTURE2D_DESC desc_ret;
	HRESULT hrCapture = _CaptureTexture(
		_poDeviceContext,
		pBackBuffer,
		desc_ret,
		pStagingBackBuffer);

	if (pStagingBackBuffer)
	{
		D3D11_TEXTURE2D_DESC description;
		pStagingBackBuffer->GetDesc(&description);
		const int width = description.Width;
		const int height = description.Height;
		unsigned int size = width * height * 4;

		D3D11_MAPPED_SUBRESOURCE resource;
		unsigned int subresource = D3D11CalcSubresource(0, 0, 0);
		HRESULT hr = _poDeviceContext->Map(pStagingBackBuffer, subresource, D3D11_MAP::D3D11_MAP_READ, 0, &resource);


		const int pitch = width << 2;

		unsigned char* source = static_cast<unsigned char*>(resource.pData);
		vector<unsigned char> vecCaptured(height * (width * 4));

		int index = 0;
		for (int i = 0; i < height; ++i)
		{
			memcpy(&vecCaptured[index], source, width * 4);
			source += resource.RowPitch;
			index += width * 4;
		}

		_poDeviceContext->Unmap(pStagingBackBuffer, 0);

		int enccode_result = lodepng::encode(_strFileName,
											 vecCaptured, width, height, LodePNGColorType::LCT_RGBA, 8U);

		string error = lodepng_error_text(enccode_result);

		pBackBuffer->Release();
		pStagingBackBuffer->Release();

		return true && (enccode_result == 0);
	}

	return false;

//
//	//ID3D11Texture2D* pBackBuffer = nullptr;
//ID3D11Texture2D* pStagingBackBuffer = nullptr;
////HRESULT hr = m_oSwapChainData.m_pSwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
//
//D3D11_TEXTURE2D_DESC desc_ret;
//HRESULT hrCapture = _CaptureTexture(
//	_poDeviceContext,
//	this->m_cpoTexture.Get(),
//	desc_ret,
//	pStagingBackBuffer);
//
//if (pStagingBackBuffer)
//{
//	D3D11_TEXTURE2D_DESC description;
//	pStagingBackBuffer->GetDesc(&description);
//	const int width = description.Width;
//	const int height = description.Height;
//	unsigned int size = width * height * 4;
//
//	//	ID3D11Texture2D* pNewTexture = NULL;
//
//	//	D3D11_TEXTURE2D_DESC description;
//	//	pBackBuffer->GetDesc(&description);
//	//	const int width = description.Width;
//	//	const int height = description.Height;
//	//	description.BindFlags = 0;
//	//	description.SampleDesc.Count = 1;
//	//	description.SampleDesc.Quality = 0;
//	//	description.CPUAccessFlags = D3D11_CPU_ACCESS_READ| D3D11_CPU_ACCESS_WRITE;
//	//	description.Usage = D3D11_USAGE_STAGING;
//	//	unsigned int size = width * height * 4;
//
//	//	HRESULT hrTex = m_cpoDevice->CreateTexture2D(&description, NULL, &pNewTexture);
//	//	HRESULT hrTexTemp = m_cpoDevice->CreateTexture2D(&description, NULL, &pTempTexture);
//	//	if (pNewTexture && hrTexTemp)
//	//	{
//	//		//m_poContext->CopyResource(pTempTexture, pBackBuffer);
//
//			//unsigned int subresource_resolve = D3D11CalcSubresource(0, 0, description.MipLevels);
//			//m_poContext->ResolveSubresource(pNewTexture, subresource_resolve, pBackBuffer, 0, description.Format);
//			////m_poContext->ResolveSubresource(pNewTexture,0, pSurface, 0, description.Format);
//			//
//	D3D11_MAPPED_SUBRESOURCE resource;
//	unsigned int subresource = D3D11CalcSubresource(0, 0, 0);
//	//HRESULT hr = m_poContext->Map(pNewTexture, subresource, D3D11_MAP::D3D11_MAP_READ, 0, &resource);
//	HRESULT hr = _poDeviceContext->Map(pStagingBackBuffer, subresource, D3D11_MAP::D3D11_MAP_READ, 0, &resource);
//
//	//resource.pData; // TEXTURE DATA IS HERE
//
//	const int pitch = width << 2;
//
//	unsigned char* source = static_cast<unsigned char*>(resource.pData);
//	vector<unsigned char> vecCaptured(height * (width * 4));
//
//	int index = 0;
//	for (int i = 0; i < height; ++i)
//	{
//		memcpy(&vecCaptured[index], source, width * 4);
//		source += resource.RowPitch;
//		index += width * 4;
//	}
//
//	_poDeviceContext->Unmap(pStagingBackBuffer, 0);
//
//	/*	m_captureSize = size;
//		m_captureWidth = width;
//		m_captureHeight = height;*/
//
//	time_t rawtime;
//
//	struct tm timeinfo;
//	char buffer[80];
//
//	time(&rawtime);
//	localtime_s(&timeinfo, &rawtime);
//
//	strftime(buffer, sizeof(buffer), "%d_%m_%Y_%H_%M_%S", &timeinfo);
//	std::string strTime(buffer);
//
//	/*QString qstrExePath = QCoreApplication::applicationDirPath() + "/screen_shots/";
//	QDir qDir;
//	qDir.mkdir(qstrExePath);*/
//
//	/*string strFíleName = qstrExePath.toStdString() +
//		_strPrefix + "RMEditor_Screenshot" + strTime + ".PNG";*/
//
//	int enccode_result = lodepng::encode(_strFileName,
//										 vecCaptured, width, height, LodePNGColorType::LCT_RGBA, 8U);
//
//	string error = lodepng_error_text(enccode_result);
//
//	//return true;
//
//	//pBackBuffer->Release();
//	pStagingBackBuffer->Release();
//}
//
//return false;
}
	  

//#include "TextureView.h"
//#include "Scene.h"
//#include "vertex_layouts.h"
//#include "..\ToolsLib\lodepng.h"
//#include "DXMeshBuffer.h"
//
//shared_ptr<rendutil::DxTextureView> rendutil::DxTextureView::create(rendutil::Scene* _poScene, bool _bCreateDepth, UINT _width, UINT _height)
//{
//	auto po = make_shared< rendutil::DxTextureView>();
//
//	po->init(_poScene, _width, _height);
//	po->initDepthStencilView(_poScene, _width, _height);
//	po->resetQuad(_poScene, _width, _height);
//
//	return po;
//}
//
////void rendutil::DxTextureView::renderQuadToTexture(rendutil::Scene* _poScene, rendutil::DxTextureView* _poTextureDest)
////{
////	ID3D11ShaderResourceView* SRV[1] = { m_cpoShaderResourceView.Get() };
////
////	_poScene->getDeviceContext()->PSSetShaderResources(42, 1, SRV);
////
////	_poScene->getDeviceContext()->OMSetRenderTargets(1, _poTextureDest->m_cpoRenderTargetView.GetAddressOf(), NULL);
////
////	renderQuad(_poScene);
////
////	ID3D11RenderTargetView* NULL_RTV[1] = { NULL };
////	_poScene->getDeviceContext()->OMSetRenderTargets(1, NULL_RTV, NULL);
////}
//
//void rendutil::DxTextureView::renderQuad(rendutil::Scene* _poScene, float _width, float _height)
//{
//	//	m_oBlendState.setAsActive(_poScene);
//
//	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
//		Matrix::CreateOrthographic(2.f / _width, 2.f / _height, 0.01, 25).Transpose();
//
//	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
//	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
//	_poScene->update_VS_ConstBuffer_PerFrame();
//
//	m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
//
//	ID3D11ShaderResourceView* PQuadSrv[1] = {
//		m_cpoShaderResourceView.Get()
//	};
//
//	m_spoQuadMesh->m_stride = sizeof(QuadVertex);
//
//	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);
//
//	m_spoQuadMesh->drawMesh_Bare(_poScene);
//
//	//_poScene->getDeviceContext()->IASetVertexBuffers(0, 1,
//	//	m_spoQuadMesh->m_cpoVertexBuffer.GetAddressOf(),
//	//	&m_spoQuadMesh->m_stride, &m_spoQuadMesh->m_offset);
//
//	//_poScene->getDeviceContext()->IASetIndexBuffer(
//	//	//m_spoQuadMesh->indexBuffer(),
//	//	m_spoQuadMesh->m_cpoIndexBuffer.Get(),
//	//	m_spoQuadMesh->EIndexFormat, 0
//	//);
//
//	/////////////////////////////////////////////////////////////////////////////////////////////
//	//// Draw triangles
//	/////////////////////////////////////////////////////////////////////////////////////////////
//	//_poScene->getDeviceContext()->IASetPrimitiveTopology(m_spoQuadMesh->ETopology);
//	//_poScene->getDeviceContext()->DrawIndexed(m_spoQuadMesh->indexCount(), 0, 0);
//	/////////////////////////////////////////////////////////////////////////////////////////////
//
//	TextureView::unbindShaderResourceViews(_poScene, 0, 1);
//}
//
//void rendutil::DxTextureView::renderQuadToTexture(rendutil::Scene* _poScene, const TextureView* _poDest, float _width, float _height)
//{
//	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);
//
//	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection = Matrix::CreateOrthographic(2.f / _width, 2.f / _height, 0.01f, 25.f).Transpose();
//	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
//	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
//	_poScene->update_VS_ConstBuffer_PerFrame();
//
//	m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
//
//	ID3D11ShaderResourceView* PQuadSrv[1] = {
//		m_cpoShaderResourceView.Get()
//	};
//
//	m_spoQuadMesh->m_stride = sizeof(QuadVertex);
//
//	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);
//
//	m_spoQuadMesh->drawMesh_Bare(_poScene);
//
//	//_poScene->getDeviceContext()->IASetVertexBuffers(0, 1,
//	//	m_spoQuadMesh->m_cpoVertexBuffer.GetAddressOf(),
//	//	&m_spoQuadMesh->m_stride, &m_spoQuadMesh->m_offset);
//
//	//_poScene->getDeviceContext()->IASetIndexBuffer(
//	//	//m_spoQuadMesh->indexBuffer(),
//	//	m_spoQuadMesh->m_cpoIndexBuffer.Get(),
//	//	m_spoQuadMesh->EIndexFormat, 0
//	//);
//
//	/////////////////////////////////////////////////////////////////////////////////////////////
//	//// Draw triangles
//	/////////////////////////////////////////////////////////////////////////////////////////////
//	//_poScene->getDeviceContext()->IASetPrimitiveTopology(m_spoQuadMesh->ETopology);
//	//_poScene->getDeviceContext()->DrawIndexed(m_spoQuadMesh->indexCount(), 0, 0);
//	/////////////////////////////////////////////////////////////////////////////////////////////
//
//	TextureView::unbindShaderResourceViews(_poScene, 0, 1);
//}
//
//void rendutil::DxTextureView::copyTo(rendutil::Scene* _poScene, TextureView* _poDest, rendutil::Shader* _poShader, bool _bScaleUV, float _width, float _height)
//{
//	D3D11_VIEWPORT vp;
//	vp.Width = m_textureWidth;
//	vp.Height = m_textureHeight;
//	vp.MinDepth = 0.f;
//	vp.MaxDepth = 1.f;
//	vp.TopLeftX = 0;
//	vp.TopLeftY = 0;
//	_poScene->getDeviceContext()->RSSetViewports(1, &vp);
//
//	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);
//
//	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
//		Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
//	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
//	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
//	//_poScene->m_VS_PerFrame_ConstantBuffer.v4Offset = { _width, _height };
//	_poScene->update_VS_ConstBuffer_PerFrame();
//
//	resetQuad(_poScene, m_textureWidth, m_textureHeight);
//
//	if (_bScaleUV)
//	{
//		//// calcuate the UV scaling:
//		//// size 2 to size 1, UV_div = 1 / 2 = 0.5
//		//m_u_div = (float)_poDest->getWidth() / (float)m_width;
//		//m_v_div = (float)_poDest->getWidth() / (float)m_width;
//
//		//_poScene->m_PS_PerMesh_ConstantBuffer.decal_u = m_u_div;
//		//_poScene->m_PS_PerMesh_ConstantBuffer.decal_v = m_v_div;
//
//		_poDest->m_v_div = 1.0;
//		_poDest->m_u_div = 1.0;
//	}
//	else
//	{
//		_poScene->m_PS_PerMesh_ConstantBuffer.decal_u = m_v_div;
//		_poScene->m_PS_PerMesh_ConstantBuffer.decal_v = m_u_div;
//	}
//
//	_poScene->update_PS_ConstBuffer_PerMesh();
//
//	_poShader->setAllShadersAsActive(_poScene->getDeviceContext());
//
//	ID3D11ShaderResourceView* PQuadSrv[1] = {
//		m_cpoShaderResourceView.Get()
//	};
//
//	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)_poDest->m_textureWidth, (float)_poDest->m_textureHeight);
//	m_spoQuadMesh->m_stride = sizeof(QuadVertex);
//
//	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);
//
//	m_spoQuadMesh->drawMesh_Strip(_poScene);
//
//	TextureView::unbindShaderResourceViews(_poScene, 0, 1);
//	TextureView::unbindRenderTargetView(_poScene, 0, 1);
//}
//
//void rendutil::DxTextureView::renderTo(rendutil::Scene* _poScene, TextureView* _poDest, bool _bScaleUV, float _width, float _height)
//{
//	copyTo(_poScene, _poDest, m_spoQuadShader.get(), _bScaleUV, _width, _height);
//}
//
//void rendutil::DxTextureView::downSample(rendutil::Scene* _poScene, TextureView* _poDest)
//{
//	D3D11_VIEWPORT vp;
//	vp.Width = (float)_poDest->m_textureWidth;
//	vp.Height = (float)_poDest->m_textureHeight;
//	vp.MinDepth = 0.f;
//	vp.MaxDepth = 1.f;
//	vp.TopLeftX = 0;
//	vp.TopLeftY = 0;
//	_poScene->getDeviceContext()->RSSetViewports(1, &vp);
//
//	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);
//
//	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
//		Matrix::CreateOrthographic(_poDest->m_textureWidth, _poDest->m_textureHeight, 0.01, 25).Transpose();
//	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
//	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
//
//	_poScene->update_VS_ConstBuffer_PerFrame();
//	resetQuad(_poScene, _poDest->m_textureWidth, _poDest->m_textureHeight);
//
//	_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
//	_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
//	_poScene->update_PS_ConstBuffer_PerMesh();
//
//	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);
//
//	_poScene->update_PS_ConstBuffer_PerMesh();
//
//	m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
//
//	ID3D11ShaderResourceView* PQuadSrv[1] = {
//		m_cpoShaderResourceView.Get()
//	};
//
//	m_spoQuadMesh->m_stride = sizeof(QuadVertex);
//
//	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);
//
//	m_spoQuadMesh->drawMesh_Strip(_poScene);
//
//	TextureView::unbindShaderResourceViews(_poScene, 0, 1);
//	TextureView::unbindRenderTargetView(_poScene, 0, 1);
//}
//
//void rendutil::DxTextureView::upSample(rendutil::Scene* _poScene, TextureView* _poDest, rendutil::Shader* _poShader)
//{
//	D3D11_VIEWPORT vp;
//	vp.Width = (float)_poDest->m_textureWidth;
//	vp.Height = (float)_poDest->m_textureHeight;
//	vp.MinDepth = 0.f;
//	vp.MaxDepth = 1.f;
//	vp.TopLeftX = 0;
//	vp.TopLeftY = 0;
//	_poScene->getDeviceContext()->RSSetViewports(1, &vp);
//
//	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);
//
//	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
//		Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
//	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
//	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
//	_poScene->update_VS_ConstBuffer_PerFrame();
//
//	resetQuad(_poScene, m_textureWidth, m_textureHeight);
//
//	_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
//	_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
//	_poScene->update_PS_ConstBuffer_PerMesh();
//
//	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);
//
//	if (_poShader)
//	{
//		_poShader->setAllShadersAsActive(_poScene->getDeviceContext());
//	}
//	else
//	{
//		m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
//	}
//
//	ID3D11ShaderResourceView* PQuadSrv[1] = {
//		m_cpoShaderResourceView.Get()
//	};
//
//	m_spoQuadMesh->m_stride = sizeof(QuadVertex);
//
//	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);
//
//	m_spoQuadMesh->drawMesh_Strip(_poScene);
//
//	TextureView::unbindShaderResourceViews(_poScene, 0, 1);
//	TextureView::unbindRenderTargetView(_poScene, 0, 1);
//}
//
//void rendutil::DxTextureView::upSample_Cam(rendutil::Scene* _poScene, TextureView* _poDest, rendutil::Shader* _poShader)
//{
//	D3D11_VIEWPORT vp;
//	vp.Width = (float)_poDest->m_textureWidth;
//	vp.Height = (float)_poDest->m_textureHeight;
//	vp.MinDepth = 0.f;
//	vp.MaxDepth = 1.f;
//	vp.TopLeftX = 0;
//	vp.TopLeftY = 0;
//	_poScene->getDeviceContext()->RSSetViewports(1, &vp);
//
//	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);
//
//	//_poScene->m_VS_PerFrame_ConstantBuffer.mProjection = Matrix(_poScene->m_oSceneCamera.GetProjMatrix()).Transpose();
//	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection = Matrix(_poScene->m_oSceneCamera.GetProjMatrix()).Transpose();
//	//Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
//	/*_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix(_poScene->m_oSceneCamera.GetViewMatrix()).Transpose();
//	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix(_poScene->m_oSceneCamera.GetViewMatrix()).Invert().Transpose();
//	_poScene->update_VS_ConstBuffer_PerFrame();
//	*/	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix(_poScene->m_oSceneCamera.GetViewMatrix()).Transpose();
//	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix(_poScene->m_oSceneCamera.GetViewMatrix()).Invert().Transpose();
//	_poScene->update_VS_ConstBuffer_PerFrame();
//
//	resetQuad(_poScene, m_textureWidth, m_textureHeight);
//
//	_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
//	_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
//	_poScene->update_PS_ConstBuffer_PerMesh();
//
//	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);
//
//	if (_poShader)
//	{
//		_poShader->setAllShadersAsActive(_poScene->getDeviceContext());
//	}
//	else
//	{
//		m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
//	}
//
//	ID3D11ShaderResourceView* PQuadSrv[1] = {
//		m_cpoShaderResourceView.Get()
//	};
//
//	m_spoQuadMesh->m_stride = sizeof(QuadVertex);
//
//	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);
//
//	m_spoQuadMesh->drawMesh_Strip(_poScene);
//
//	TextureView::unbindShaderResourceViews(_poScene, 0, 1);
//	TextureView::unbindRenderTargetView(_poScene, 0, 1);
//}
//
//void rendutil::DxTextureView::drawDepthBuffer(rendutil::Scene* _poScene, TextureView* _poDest)
//{
//	D3D11_VIEWPORT vp;
//	vp.Width = (float)_poDest->m_textureWidth;
//	vp.Height = (float)_poDest->m_textureHeight;
//	vp.MinDepth = 0.f;
//	vp.MaxDepth = 1.f;
//	vp.TopLeftX = 0;
//	vp.TopLeftY = 0;
//	_poScene->getDeviceContext()->RSSetViewports(1, &vp);
//
//	_poDest->setActiveDestRenderTarget_DSV_NULL(_poScene);
//
//	_poScene->m_VS_PerFrame_ConstantBuffer.mProjection =
//		Matrix::CreateOrthographic(m_textureWidth, m_textureHeight, 0.01, 25).Transpose();
//	_poScene->m_VS_PerFrame_ConstantBuffer.mView = Matrix::Identity;
//	_poScene->m_VS_PerFrame_ConstantBuffer.mViewInverse = Matrix::Identity;
//	_poScene->update_VS_ConstBuffer_PerFrame();
//
//	resetQuad(_poScene, m_textureWidth, m_textureHeight);
//
//	_poScene->m_PS_PerMesh_ConstantBuffer.u_div = (float)m_textureWidth / (float)_poDest->m_textureWidth;
//	_poScene->m_PS_PerMesh_ConstantBuffer.v_div = (float)m_textureHeight / (float)_poDest->m_textureHeight;
//	_poScene->update_PS_ConstBuffer_PerMesh();
//
//	_poScene->updatePS_Light_ConstBuffer_PerFrame((float)m_textureWidth, (float)m_textureHeight);
//
//	//if (_poShader)
//	//{
//	//	_poShader->setShader(_poScene->getDeviceContext());
//	//}
//	//else
//	{
//		m_spoQuadShader->setAllShadersAsActive(_poScene->getDeviceContext());
//	}
//
//	ID3D11ShaderResourceView* PQuadSrv[1] = {
//		m_cpoDepthShaderResourceView.Get()
//		//m_cpoDepthShaderResourceView.Get()
//	};
//
//	m_spoQuadMesh->m_stride = sizeof(QuadVertex);
//
//	_poScene->getDeviceContext()->PSSetShaderResources(0, 1, PQuadSrv);
//
//	m_spoQuadMesh->drawMesh_Strip(_poScene);
//
//	TextureView::unbindShaderResourceViews(_poScene, 0, 1);
//	TextureView::unbindRenderTargetView(_poScene, 0, 1);
//}
//
//void rendutil::DxTextureView::resolveDepthBuffer(rendutil::Scene* _poScene)
//{
//	//_poScene->getDeviceContext()->ResolveSubresource
//	//(
//	//	m_cpoTexture.Get(),
//	//	D3D11CalcSubresource(0, 0, 1),
//	//	m_cpoDepthStencilTexture.Get(),
//	//	D3D11CalcSubresource(0, 0, 1),
//	//	DXGI_FORMAT_R32_TYPELESS
//	//);
//}
//
//void rendutil::DxTextureView::unbindShaderResourceViews(rendutil::Scene* _poScene, UINT _slot, UINT _count)
//{
//	std::vector<ID3D11ShaderResourceView*>  vecSRV_NULL(_count, NULL);
//	_poScene->getDeviceContext()->PSSetShaderResources(_slot, _count, vecSRV_NULL.data());
//}
//
//void rendutil::DxTextureView::unbindRenderTargetView(rendutil::Scene* _poScene, UINT _slot, UINT _count)
//{
//	vector<ID3D11RenderTargetView*> vecRTV_NULL(_count, NULL);
//
//	UINT count = (D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT < _count) ? D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT : _count;
//	_poScene->getDeviceContext()->OMSetRenderTargets(count, vecRTV_NULL.data(), NULL);
//}
//
//void rendutil::DxTextureView::setActiveDestRenderTargetAndDSV(rendutil::Scene* _poScene) const
//{
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		1,
//		m_cpoRenderTargetView.GetAddressOf(), m_poDepthStencilView.Get()
//	);
//}
//
//void rendutil::DxTextureView::setActiveDestRenderTargetAndDSVNULL(rendutil::Scene* _poScene) const
//{
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		1,
//		m_cpoRenderTargetView.GetAddressOf(), NULL
//	);
//}
//
//void rendutil::DxTextureView::setActiveDestRenderTargetAndDSV_RAW(rendutil::Scene* _poScene) const
//{
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		1,
//		&m_poRenderTargetView_RAW, m_cpoDepthStencilView_RAW
//	);
//}
//
//void rendutil::DxTextureView::setActiveDestRenderTargetOnly_RAW(rendutil::Scene* _poScene) const
//{
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		1,
//		&m_poRenderTargetView_RAW, NULL
//	);
//}
//
//void rendutil::DxTextureView::setActiveDestRenderTarget_DSV_NULL(rendutil::Scene* _poScene) const
//{
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		1,
//		m_cpoRenderTargetView.GetAddressOf(), NULL
//	);
//}
//
//void rendutil::DxTextureView::setActiveDestRenderTarget_DSV_NULL_RAW(rendutil::Scene* _poScene) const
//{
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		1,
//		&m_poRenderTargetView_RAW, NULL
//	);
//}
//
//void rendutil::DxTextureView::setActiveDSVOnly_RAW(rendutil::Scene* _poScene)
//{
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		1,
//		NULL, m_cpoDepthStencilView_RAW
//	);
//}
//
//void rendutil::DxTextureView::setActiveDSVOnly(rendutil::Scene* _poScene)
//{
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		0,
//		NULL, m_poDepthStencilView.Get()
//	);
//}
//
///*void rendutil::RenderTarget2D::setActiveDestRenderTarget_RAW(rendutil::Scene* _poScene)
//{
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		1,
//		&m_cpoRenderTargetView, m_poDepthStencilView
//	);
//}*/
//
//void rendutil::DxTextureView::clearPixelsAndDepth(rendutil::Scene* _poScene, SimpleMath::Color vColor)
//{
//	_poScene->getDeviceContext()->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);
//
//	_poScene->getDeviceContext()->ClearDepthStencilView(
//		m_poDepthStencilView.Get(),
//		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
//		1.0f,
//		0);
//}
//
//void rendutil::DxTextureView::clearDepthOnly(rendutil::Scene* _poScene)
//{
//	_poScene->getDeviceContext()->ClearDepthStencilView(
//		m_poDepthStencilView.Get(),
//		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
//		1.0f,
//		0);
//}
//
//void rendutil::DxTextureView::clearAndDepth_RAW(rendutil::Scene* _poScene, SimpleMath::Color vColor)
//{
//	//	_poScene->getDeviceContext()->ClearRenderTargetView(m_poRenderTargetView_RAW, vColor);
//	_poScene->getDeviceContext()->ClearRenderTargetView(_poScene->m_poScreenBuffer->getRenderTargetView(), vColor);
//
//	_poScene->getDeviceContext()->ClearDepthStencilView(
//		m_cpoDepthStencilView_RAW,
//		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
//		1.0f,
//		0);
//}
//
//void rendutil::DxTextureView::clearDepthOnly_RAW(rendutil::Scene* _poScene)
//{
//	_poScene->getDeviceContext()->ClearDepthStencilView(
//		m_cpoDepthStencilView_RAW,
//		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
//		1.0f,
//		0);
//}
//
///// <summary>
///// Operates on non ComPtr owner pointers, clearPixels framebuffer and depth buffer/stencil
///// </summary>
///// <param name="_poScene">Scene ptr</param>
///// <param name="vColor">RGBA float</param>
////void rendutil::RenderTarget2D::clearAndDepth_RAW(rendutil::Scene* _poScene, SimpleMath::Color vColor)
////{
////	//#if defined(_DEBUG)
////	//	char szDepth_Stencil_View[] = "DEPTH Stencil View";
////	//	m_poDepthStencilView_RAW->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szDepth_Stencil_View) - 1, szDepth_Stencil_View);
////	//
////	//	char szDepth_SRV_NAME[] = "DEPTH Shader Resource View";
////	//	m_poDepthShaderResourceView_RAW->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szDepth_SRV_NAME) - 1, szDepth_SRV_NAME);
////	//#endif
////
////	_poScene->getDeviceContext()->ClearRenderTargetView(m_poRenderTargetView_RAW, vColor);
////
////	_poScene->getDeviceContext()->ClearDepthStencilView(
////		m_poDepthStencilView,
////		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
////		1.0f,
////		0);
////}
//
//void rendutil::DxTextureView::clearPixels(rendutil::Scene* _poScene, SimpleMath::Color vColor)
//{
//	_poScene->getDeviceContext()->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);
//}
//
//void rendutil::DxTextureView::clearPixels(ID3D11DeviceContext* _pDC, SimpleMath::Color vColor)
//{
//	_pDC->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);
//}
//
//void rendutil::DxTextureView::setRenderTargetView_RAW(ID3D11RenderTargetView* _poRenderTargetView)
//{
//	m_poRenderTargetView_RAW = _poRenderTargetView;
//}
//
//void rendutil::DxTextureView::setShaderResourceView_RAW(ID3D11ShaderResourceView* _poShaderResourceView)
//{
//	m_cpoDepthShaderResourceView_RAW = _poShaderResourceView;
//}
//
//void rendutil::DxTextureView::setDepthStencilView_RAW(ID3D11DepthStencilView* _poDepthStenticlView)
//{
//	m_cpoDepthStencilView_RAW = _poDepthStenticlView;
//}
//
//void rendutil::DxTextureView::init_views(
//	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& _cpoRTV,
//	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& _cpoDepthView,
//	const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthTexture,
//	const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthSRV,
//	const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthView_ReadOnly
//)
//{
//	if (_cpoRTV)
//	{
//		m_cpoRenderTargetView.Detach();
//		m_cpoRenderTargetView.Attach(_cpoRTV.Get());
//	}
//
//	if (_cpoRTV)
//	{
//		m_poDepthStencilView.Detach();
//		m_poDepthStencilView.Attach(_cpoDepthView.Get());
//	}
//}
//
//bool rendutil::DxTextureView::init_ms(rendutil::Scene* _poScene, UINT _width, UINT _height, const char* _szObjectName)
//{
//	HRESULT hr = S_OK;
//
//	m_textureWidth = _width;
//	m_textureHeight = _height;
//
//	///////////////////////// Map's Texture
//	// Initialize the  texture description.
//	D3D11_TEXTURE2D_DESC textureDesc;
//	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//
//	ZeroMemory(&textureDesc, sizeof(textureDesc));
//	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
//
//	// Setup the texture description.
//	// We will have our map be a square
//	// We will need to have this texture bound as a renderQuad target AND a shader resource
//
//	// get client dimension = renderQuad target dimensions
//
//	/*RECT rc = getClientRect();
//	UINT _width = rc.right;
//	UINT _height = rc.bottom;*/
//
//	//_poScene->getRT
//
//		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	//D3D11_TEXTURE2D_DESC textureDesc;
//	textureDesc.Width = _width;
//	textureDesc.Height = _height;
//	textureDesc.MipLevels = 1;
//	textureDesc.ArraySize = 1;
//	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//
//	textureDesc.SampleDesc.Count = 1;
//	textureDesc.SampleDesc.Quality = 0;
//
//	//textureDesc.SampleDesc = m_oSampleDesc;
//	textureDesc.Usage = D3D11_USAGE_DEFAULT;
//	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//	textureDesc.CPUAccessFlags = 0;
//	textureDesc.MiscFlags = 0;
//
//	// Create the texture
//	hr = _poScene->getDevice()->CreateTexture2D(&textureDesc, NULL, &m_cpoTexture);
//	assert(SUCCEEDED(hr));
//
//	/////////////////////// Map's Render Target
//	// Setup the description of the renderQuad target view.
//	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
//	renderTargetViewDesc.Format = textureDesc.Format;
//	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;;// D3D11_RTV_DIMENSION_TEXTURE2D;
//	renderTargetViewDesc.Texture2D.MipSlice = 0;
//
//	// Create the renderQuad target view.
//	m_cpoRenderTargetView.Detach();
//	hr = _poScene->getDevice()->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
//	assert(SUCCEEDED(hr));
//
//	//CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView;
//
//	//D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//	shaderResourceViewDesc.Format = textureDesc.Format;
//	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMS;
//	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
//	shaderResourceViewDesc.Texture2D.MipLevels = 1;
//
//	// Create the shader resource view.
//
//	hr = _poScene->getDevice()->CreateShaderResourceView(m_cpoTexture.Get(),
//		&shaderResourceViewDesc, &m_cpoShaderResourceView);
//	assert(SUCCEEDED(hr));
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	return true;
//}
//
//bool rendutil::DxTextureView::init(
//	rendutil::Scene* _poScene,
//	UINT _width, UINT _height,
//	DXGI_FORMAT _Format,
//	UINT uiSampleCount,
//	const char* _szObjectName
//)
//{
//	resetQuad(_poScene, _width, _height);
//
//	return init(
//		_poScene->getDevice(),
//		_width, _height,
//		_Format,
//		uiSampleCount,
//		_szObjectName
//	);
//}
//
//bool rendutil::DxTextureView::init(
//	ID3D11Device* _poDevice,
//	UINT _width, UINT _height,
//	DXGI_FORMAT _Format,
//	UINT uiSampleCount,
//	const char* _szObjectName
//)
//{
//	HRESULT hr = S_OK;
//
//	m_textureWidth = _width;
//	m_textureHeight = _height;
//
//	///////////////////////// Map's Texture
//	// Initialize the  texture description.
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//
//	ZeroMemory(&m_textureDesc, sizeof(m_textureDesc));
//	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
//
//	// Setup the texture description.
//	// We will have our map be a square
//	// We will need to have this texture bound as a renderQuad target AND a shader resource
//
//	// get client dimension = renderQuad target dimensions
//
//	/*RECT rc = getClientRect();
//	UINT _width = rc.right;
//	UINT _height = rc.bottom;*/
//
//	//_poScene->getRT
//
//		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	//D3D11_TEXTURE2D_DESC textureDesc;
//	m_textureDesc.Width = _width;
//	m_textureDesc.Height = _height;
//	m_textureDesc.MipLevels = 1;
//	m_textureDesc.ArraySize = 1;
//	m_textureDesc.Format = _Format;
//
//	m_textureDesc.SampleDesc.Count = uiSampleCount;
//	m_textureDesc.SampleDesc.Quality = 0;
//
//	//textureDesc.SampleDesc = m_oSampleDesc;
//	m_textureDesc.Usage = D3D11_USAGE_DEFAULT;
//
//	if (_Format == DXGI_FORMAT_R32_TYPELESS)
//		m_textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//	else
//		m_textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//
//	m_textureDesc.CPUAccessFlags = 0;
//	m_textureDesc.MiscFlags = 0;
//
//	// Create the texture
//	hr = _poDevice->CreateTexture2D(&m_textureDesc, NULL, &m_cpoTexture);
//	assert(SUCCEEDED(hr));
//
//#if defined (DEBUG) || defined(_DEBUG)
//	string strName = "Texture32";
//	m_cpoTexture->SetPrivateData(WKPDID_D3DDebugObjectName, strName.size(), strName.data());
//#endif
//
//	/////////////////////// Map's Render Target
//	// Setup the description of the renderQuad target view.
//	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
//	renderTargetViewDesc.Format = m_textureDesc.Format;
//	renderTargetViewDesc.ViewDimension =
//		(m_textureDesc.SampleDesc.Count > 1) ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
//
//	renderTargetViewDesc.Texture2D.MipSlice = 0;
//
//	// Create the renderQuad target view.
//	hr = _poDevice->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
//	assert(SUCCEEDED(hr));
//
//#if defined (DEBUG) || defined(_DEBUG)
//	strName = "RTV";
//	m_cpoRenderTargetView->SetPrivateData(WKPDID_D3DDebugObjectName, strName.size(), strName.data());
//#endif
//
//	//CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView;
//
//	//D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//	shaderResourceViewDesc.Format = m_textureDesc.Format;
//	shaderResourceViewDesc.ViewDimension =
//		(m_textureDesc.SampleDesc.Count > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
//	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
//	shaderResourceViewDesc.Texture2D.MipLevels = 1;
//
//	// Create the shader resource view.
//
//	hr = _poDevice->CreateShaderResourceView(m_cpoTexture.Get(),
//		&shaderResourceViewDesc, &m_cpoShaderResourceView);
//	assert(SUCCEEDED(hr));
//
//#if defined (DEBUG) || defined(_DEBUG)
//	strName = "SRV";
//	m_cpoShaderResourceView->SetPrivateData(WKPDID_D3DDebugObjectName, strName.size(), strName.data());
//#endif
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	return true;
//}
//
//bool rendutil::DxTextureView::initDepthStencilView(
//	Scene* _poScene,
//	UINT _width, UINT _height,
//	DXGI_FORMAT Format,
//	UINT _uiSampleCount,
//	const char* _szObjectName)
//{
//	m_textureWidth = _width;
//	m_textureHeight = _height;;
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// Create Depth buffer TEXTURE
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	HRESULT hr = S_OK;
//	D3D11_TEXTURE2D_DESC depthTextureDesc;
//	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
//	depthTextureDesc.Width = _width;
//	depthTextureDesc.Height = _height;
//	depthTextureDesc.MipLevels = 1;
//	depthTextureDesc.ArraySize = 1;
//
//	depthTextureDesc.SampleDesc = { _uiSampleCount, 0 };
//	depthTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;//  DXGI_FORMAT_R24G8_TYPELESS; //DXGI_FORMAT_D24_UNORM_S8_UINT;
//	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
//	depthTextureDesc.SampleDesc.Count = _uiSampleCount;
//	depthTextureDesc.SampleDesc.Quality = 0;
//	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
//
//	hr = _poScene->getDevice()->CreateTexture2D(&depthTextureDesc, NULL,
//		m_cpoDepthStencilTexture.GetAddressOf());
//	assert(SUCCEEDED(hr));
//
//	string szDepthTextureName = string("DepthTeX_") + std::to_string(sm_NextId);
//	m_cpoDepthStencilTexture->SetPrivateData(WKPDID_D3DDebugObjectName,
//		szDepthTextureName.size() - 1, szDepthTextureName.data());
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// Create Depth  SHADER RESOURCE VIEW
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT; //DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
//	shaderResourceViewDesc.ViewDimension =
//		(_uiSampleCount > 1) ?
//		D3D11_SRV_DIMENSION_TEXTURE2DMS :
//		D3D11_SRV_DIMENSION_TEXTURE2D;
//
//	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
//	shaderResourceViewDesc.Texture2D.MipLevels = 1;
//
//	// Create the shader resource view.
//	hr = _poScene->getDevice()->CreateShaderResourceView(m_cpoDepthStencilTexture.Get(),
//		&shaderResourceViewDesc, m_cpoDepthShaderResourceView.GetAddressOf());
//	assert(SUCCEEDED(hr));
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// Create Depth Stencil VIEW
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
//	::ZeroMemory(&dsvDesc, sizeof(dsvDesc));
//
//	dsvDesc.Flags = 0;
//	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
//	dsvDesc.ViewDimension =
//		(_uiSampleCount > 1) ?
//		D3D11_DSV_DIMENSION_TEXTURE2DMS :
//		D3D11_DSV_DIMENSION_TEXTURE2D;
//
//	dsvDesc.Texture2DArray.ArraySize = 1;
//	dsvDesc.Texture2DArray.FirstArraySlice = 0;
//	dsvDesc.Texture2DArray.MipSlice = 0;
//
//	hr = _poScene->getDevice()->CreateDepthStencilView(m_cpoDepthStencilTexture.Get(), &dsvDesc,
//		m_poDepthStencilView.GetAddressOf());
//	assert(SUCCEEDED(hr));
//
//	string szDepthStencilViewName = string("DSV_") + std::to_string(sm_NextId);
//	m_poDepthStencilView->SetPrivateData(WKPDID_D3DDebugObjectName,
//		szDepthStencilViewName.size() - 1, szDepthStencilViewName.data());
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// Create READ ONLY Depth Stencil VIEW
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	dsvDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
//	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // DXGI_FORMAT_D24_UNORM_S8_UINT;
//	hr = _poScene->getDevice()->CreateDepthStencilView(m_cpoDepthStencilTexture.Get(), &dsvDesc,
//		m_cpoDepthStencilView_ReadOnly.GetAddressOf());
//	assert(SUCCEEDED(hr));
//
//	/************************
//	* 	Debug naming
//	************************/
//	string szDepthStencilViewNameReadOny = "DSV_RO_" + std::to_string(sm_NextId);
//	m_cpoDepthStencilView_ReadOnly->SetPrivateData(WKPDID_D3DDebugObjectName,
//		szDepthStencilViewNameReadOny.size() - 1, szDepthStencilViewNameReadOny.data());
//
//	///*=============================================================
//	//	Create Stencil
//	//=============================================================*/
//	//D3D11_DEPTH_STENCIL_DESC dsDesc;
//	//// Depth initLog parameters
//	//dsDesc.DepthEnable = true;
//	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
//
//	//// Stencil initLog parameters
//	//dsDesc.StencilEnable = false;
//	//dsDesc.StencilReadMask = 0x00;
//	//dsDesc.StencilWriteMask = 0x00;
//
//	//// Stencil operations if pixel is front-facing
//	//dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//	//dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	//dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//	//// Stencil operations if pixel is back-facing
//	//dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	//dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
//	//dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//	//// Create depth stencil state
//	//ID3D11DepthStencilState* pDSState;
//	//hr = _poScene->getDevice()->CreateDepthStencilState(&dsDesc, &m_poDepthStencilState_On);
//
//	//dsDesc.DepthEnable = false;
//	//hr = _poScene->getDevice()->CreateDepthStencilState(&dsDesc, &m_poDepthStencilState_Off);
//
//	//assert(SUCCEEDED(hr));
//
//	//_poScene->getDeviceContext()->OMSetDepthStencilState(m_poDepthStencilState_On, 1);
//	//m_width = _width;
//	//m_height = _height;
//
//	return SUCCEEDED(hr);
//}
//
////bool rendutil::RenderTarget2D::initWithFrameBuffer(rendutil::Scene* _poScene, UINT _width, UINT _height)
////{
////	//m_cpoRenderTargetView = _poScene->m_cpoRenderTargetView;
////	//m_cpoDepthStencilView = _poScene->m_poDepthStencilView;
////
////	m_poRenderTargetView_RAW = _poScene->m_cpoRenderTargetView.Get();
////	m_poDepthStencilView = _poScene->m_poDepthStencilView.Get();
////
////	return true;
////}
//
//bool rendutil::DxTextureView::init_ms(rendutil::Scene* _poScene, UINT Width, UINT Height, DXGI_FORMAT Format, DXGI_SAMPLE_DESC SampleDesc, D3D11_USAGE Usage, UINT BindFlags)
//{
//	return
//		SUCCEEDED(
//			init_impl(
//				_poScene,
//				Width,
//				Height,
//				1,
//				1,
//				Format,
//				SampleDesc,
//				Usage,
//				BindFlags,
//				0,
//				0
//			)
//		);
//}
//
////bool rendutil::RenderTarget2D::init(rendutil::Scene* _poScene, UINT Width, UINT Height, DXGI_FORMAT Format, DXGI_SAMPLE_DESC SampleDesc, D3D11_USAGE Usage, UINT BindFlags)
////{
////	HRESULT hr = S_OK;
////
////	D3D11_TEXTURE2D_DESC textureDesc;
////	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
////
////	///////////////////////// Map's Texture
////	// Initialize the  texture description.
////	ZeroMemory(&textureDesc, sizeof(textureDesc));
////	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
////
////	// Setup the texture description.
////
////	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	textureDesc.Width = Width;
////	textureDesc.Height = Height;
////	textureDesc.MipLevels = 1;
////	textureDesc.ArraySize = 1
////		textureDesc.Format = Format;
////
////	textureDesc.SampleDesc.Count = SampleDesc.Count;
////	textureDesc.SampleDesc.Quality = SampleDesc.Quality;
////
////	//textureDesc.SampleDesc = m_oSampleDesc;
////	textureDesc.Usage = Usage;
////	textureDesc.BindFlags = BindFlags;
////	textureDesc.CPUAccessFlags = 0;
////	textureDesc.MiscFlags = 0;
////
////	// Create the texture
////	hr = _poScene->getDevice()->CreateTexture2D(&textureDesc, NULL, &m_cpoTexture);
////	assert(SUCCEEDED(hr));
////
////	/////////////////////// Map's Render Target
////	// Setup the description of the renderQuad target view.
////	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
////	renderTargetViewDesc.Format = textureDesc.Format;
////	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;;// D3D11_RTV_DIMENSION_TEXTURE2D;
////	renderTargetViewDesc.Texture2D.MipSlice = 0;
////
////	// Create the renderQuad target view.
////	hr = _poScene->getDevice()->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
////	assert(SUCCEEDED(hr));
////
////	CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView;
////	shaderResourceViewDesc.Format = textureDesc.Format;
////	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
////	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
////	shaderResourceViewDesc.Texture2D.MipLevels = MipLevels;
////
////	// Create the shader resource view.
////
////	hr = _poScene->getDevice()->CreateShaderResourceView(m_cpoTexture.Get(),
////		&shaderResourceViewDesc, &m_cpoShaderResourceView);
////	assert(SUCCEEDED(hr));
////}
//
//HRESULT rendutil::DxTextureView::init_impl(rendutil::Scene* _poScene, UINT Width, UINT Height, UINT MipLevels, UINT ArraySize, DXGI_FORMAT Format, DXGI_SAMPLE_DESC SampleDesc, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags)
//{
//	m_textureWidth = Width;
//	m_textureHeight = Height;
//
//	HRESULT hr = S_OK;
//
//	D3D11_TEXTURE2D_DESC textureDesc;
//	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//
//	ZeroMemory(&textureDesc, sizeof(textureDesc));
//	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
//
//	// Setup the texture description.
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	textureDesc.Width = Width;
//	textureDesc.Height = Height;
//	textureDesc.MipLevels = MipLevels;
//	textureDesc.ArraySize = ArraySize;
//	textureDesc.Format = Format;
//
//	textureDesc.SampleDesc.Count = SampleDesc.Count;
//	textureDesc.SampleDesc.Quality = SampleDesc.Quality;
//
//	//textureDesc.SampleDesc = m_oSampleDesc;
//	textureDesc.Usage = Usage;
//	textureDesc.BindFlags = BindFlags;
//	textureDesc.CPUAccessFlags = CPUAccessFlags;
//	textureDesc.MiscFlags = MiscFlags;
//
//	// Create the texture
//	hr = _poScene->getDevice()->CreateTexture2D(&textureDesc, NULL, &m_cpoTexture);
//	assert(SUCCEEDED(hr));
//
//	/////////////////////// Map's Render Target
//	// Setup the description of the renderQuad target view.
//	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
//	renderTargetViewDesc.Format = textureDesc.Format;
//	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;;// D3D11_RTV_DIMENSION_TEXTURE2D;
//	renderTargetViewDesc.Texture2D.MipSlice = 0;
//
//	// Create the renderQuad target view.
//	hr = _poScene->getDevice()->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
//	assert(SUCCEEDED(hr));
//
//	CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView;
//	shaderResourceViewDesc.Format = textureDesc.Format;
//	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
//	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
//	shaderResourceViewDesc.Texture2D.MipLevels = MipLevels;
//
//	// Create the shader resource view.
//
//	hr = _poScene->getDevice()->CreateShaderResourceView(m_cpoTexture.Get(),
//		&shaderResourceViewDesc, &m_cpoShaderResourceView);
//	assert(SUCCEEDED(hr));
//
//	return hr;
//}
//
//bool rendutil::DxTextureView::resetQuad(rendutil::Scene* _poScene, UINT _width, UINT _height)
//{
//	m_quadWidth = (float)_width;
//	m_quadHeight = (float)_height;
//
//	m_oBlendState.create(_poScene);
//	// **********************************************************************************
//		// quad half size
//		// **********************************************************************************
//
//	// only create new once
//	if (!m_spoQuadMesh)
//	{
//		m_spoQuadMesh = make_shared<DXMeshBuffer>();
//	}
//
//	float left = -1.f / 2.0f;
//	float right = 1.f / 2.0f;
//	float top = 1.f / 2.0f;
//	float bottom = -1.f / 2.0f;
//
//	float w = m_quadWidth / 2;
//	float h = m_quadHeight / 2;
//	// Load the vertex array with data. a Quad, two triangles
//	//vector<QuadVertex> vecQuad_Vertices(6);
//	//vecQuad_Vertices[0].position = { -w, -h, 0.0f };
//	//vecQuad_Vertices[0].uv1 = { 0.0f, 1.0f };
//	////vecQuad_Vertices[0].color = { 1, 0, 0, 1 };
//
//	//vecQuad_Vertices[1].position = { -w, h, 0.0f };
//	//vecQuad_Vertices[1].uv1 = { 0.0f, 0.0f };
//	////vecQuad_Vertices[1].color = { 0, 0, 1, 1 };
//
//	//vecQuad_Vertices[2].position = { w, -h, 0.0f };
//	//vecQuad_Vertices[2].uv1 = { 1.0f, 1.0f };
//	////vecQuad_Vertices[2].color = { 0, 1, 0, 1 };
//
//	//vecQuad_Vertices[3].position = { -w, h, 0.0f };
//	//vecQuad_Vertices[3].uv1 = { 0.0f, 0.0f };
//	////vecQuad_Vertices[3].color = { 1, 0, 1, 1 };
//
//	//vecQuad_Vertices[4].position = { w, h, 0.0f };
//	//vecQuad_Vertices[4].uv1 = { 1.0f, 0.0f };
//	////vecQuad_Vertices[4].color = { 0, 1, 0, 1 };
//
//	//vecQuad_Vertices[5].position = { w, -h, 0.0f };
//	//vecQuad_Vertices[5].uv1 = { 1.0f, 1.0f };
//	////vecQuad_Vertices[5].color = { 0, 0, 1, 1 };
//
//	//vector<QuadVertex> vecQuad_Vertices_triangle_strip(3);
//	//vecQuad_Vertices[0].position = { -w, -h, 0.0f };
//	//vecQuad_Vertices[0].uv1 = { 0.0f, 1.0f };
//	////vecQuad_Vertices[0].color = { 1, 0, 0, 1 };
//
//	//vecQuad_Vertices[1].position = { -w, h, 0.0f };
//	//vecQuad_Vertices[1].uv1 = { 0.0f, 0.0f };
//	////vecQuad_Vertices[1].color = { 0, 0, 1, 1 };
//
//	//vecQuad_Vertices[2].position = { w, -h, 0.0f };
//	//vecQuad_Vertices[2].uv1 = { 1.0f, 1.0f };
//	////vecQuad_Vertices[2].color = { 0, 1, 0, 1 };
//
//	m_spoQuadMesh->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
//	m_spoQuadMesh->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
//
//	vector<QuadVertex> vecQuad_Vertices(4);
//	vecQuad_Vertices[0].position = { -w, -h, 0.0f };
//	vecQuad_Vertices[0].uv1 = { 0.0f, 1.0f };
//	//vecQuad_Vertices[0].color = { 1, 0, 0, 1 };
//
//	vecQuad_Vertices[1].position = { -w, h, 0.0f };
//	vecQuad_Vertices[1].uv1 = { 0.0f, 0.0f };
//	//vecQuad_Vertices[1].color = { 0, 0, 1, 1 };
//
//	vecQuad_Vertices[2].position = { w, -h, 0.0f };
//	vecQuad_Vertices[2].uv1 = { 1.0f, 1.0f };
//	//vecQuad_Vertices[2].color = { 0, 1, 0, 1 };
//
//	vecQuad_Vertices[3].position = { w, h, 0.0f };
//	vecQuad_Vertices[3].uv1 = { 1.0f, 0.0f };
//	//vecQuad_Vertices[4].color = { 0, 1, 0, 1 };
//
//	;
//	//vecIndex = {
//	//	0,2,1,
//	//	3,2,0
//	//};
//	vector<uint16_t> vecIndex = {
//		0,1,2,3
//	};
//
//	vector<uint16_t> vecIndex_TriangleStrip = {
//		0,2,1,
//		3,2,0
//	};
//
//	m_spoQuadMesh->createVertexBuffer(_poScene->getDevice(), sizeof(QuadVertex) * 6, nullptr,
//		D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE
//	);
//
//	//uint32_t indices[6];
//	//for (uint32_t i = 0; i < 6; i++)
//	//	indices[i] = i;
//
//	m_spoQuadMesh->createIndexBuffer(_poScene->getDevice(), vecIndex.size() * 2, nullptr,
//		D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE
//	);
//
//	// fill index buffer from CPU
//	D3D11_MAPPED_SUBRESOURCE pMappedResourceIndexBuffer;
//	_poScene->getDeviceContext()->Map(m_spoQuadMesh->m_cpoIndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pMappedResourceIndexBuffer);
//	::memcpy(pMappedResourceIndexBuffer.pData, vecIndex.data(), vecIndex.size() * 2);
//	_poScene->getDeviceContext()->Unmap(m_spoQuadMesh->m_cpoIndexBuffer.Get(), 0);
//
//	// fill vertex buffer from CPU
//	D3D11_MAPPED_SUBRESOURCE pMappedResourceVertexBuffer;
//	_poScene->getDeviceContext()->Map(m_spoQuadMesh->m_cpoVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pMappedResourceVertexBuffer);
//	::memcpy(pMappedResourceVertexBuffer.pData, vecQuad_Vertices.data(), vecQuad_Vertices.size() * sizeof(QuadVertex));
//	_poScene->getDeviceContext()->Unmap(m_spoQuadMesh->m_cpoVertexBuffer.Get(), 0);
//
//	m_spoQuadMesh->setIndexCount(vecIndex.size());
//
//	/*======================================
//	* Create Shader
//	===========================================*/
//	if (!m_spoQuadShader)
//	{
//		m_spoQuadShader
//			= rendutil::Shader::create(
//				_poScene->getDevice(),
//				layout_Quad,
//				_countof(layout_Quad),
//				LR"(vS_Quad.cso)",
//				LR"(ps_Quad.cso)"
//			);
//
//		return (m_spoQuadShader) ? true : false;
//	}
//
//	return true;
//};
//
//size_t  rendutil::DxTextureView::sm_ObjectCounter = 0;
//size_t  rendutil::DxTextureView::sm_NextId = 0;
//
//void rendutil::G_Buffer::setasRenderTargets(rendutil::Scene* _poScene)
//
//{
//	ID3D11RenderTargetView* pp_RTV_G_Buffer[2] =
//	{
//		spo_Position_Buffer->m_cpoRenderTargetView.Get(),
//		spo_Normal_Buffer->m_cpoRenderTargetView.Get()
//	};
//
//	_poScene->getDeviceContext()->OMSetRenderTargets(
//		2,
//		pp_RTV_G_Buffer,
//		_poScene->m_poDepthStencilView
//	);
//};
//
//bool rendutil::DxTextureView::saveToDisk(ID3D11DeviceContext* _poDeviceContext, const string& _strFileName)
//{
//	//ID3D11Texture2D* pBackBuffer = nullptr;
//	ID3D11Texture2D* pStagingBackBuffer = nullptr;
//	//HRESULT hr = m_oSwapChainData.m_pSwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
//
//	D3D11_TEXTURE2D_DESC desc_ret;
//	HRESULT hrCapture = _CaptureTexture(
//		_poDeviceContext,
//		this->m_cpoTexture.Get(),
//		desc_ret,
//		pStagingBackBuffer);
//
//	if (pStagingBackBuffer)
//	{
//		D3D11_TEXTURE2D_DESC description;
//		pStagingBackBuffer->GetDesc(&description);
//		const int width = description.Width;
//		const int height = description.Height;
//		unsigned int size = width * height * 4;
//
//		//	ID3D11Texture2D* pNewTexture = NULL;
//
//		//	D3D11_TEXTURE2D_DESC description;
//		//	pBackBuffer->GetDesc(&description);
//		//	const int width = description.Width;
//		//	const int height = description.Height;
//		//	description.BindFlags = 0;
//		//	description.SampleDesc.Count = 1;
//		//	description.SampleDesc.Quality = 0;
//		//	description.CPUAccessFlags = D3D11_CPU_ACCESS_READ| D3D11_CPU_ACCESS_WRITE;
//		//	description.Usage = D3D11_USAGE_STAGING;
//		//	unsigned int size = width * height * 4;
//
//		//	HRESULT hrTex = m_cpoDevice->CreateTexture2D(&description, NULL, &pNewTexture);
//		//	HRESULT hrTexTemp = m_cpoDevice->CreateTexture2D(&description, NULL, &pTempTexture);
//		//	if (pNewTexture && hrTexTemp)
//		//	{
//		//		//m_poContext->CopyResource(pTempTexture, pBackBuffer);
//
//				//unsigned int subresource_resolve = D3D11CalcSubresource(0, 0, description.MipLevels);
//				//m_poContext->ResolveSubresource(pNewTexture, subresource_resolve, pBackBuffer, 0, description.Format);
//				////m_poContext->ResolveSubresource(pNewTexture,0, pSurface, 0, description.Format);
//				//
//		D3D11_MAPPED_SUBRESOURCE resource;
//		unsigned int subresource = D3D11CalcSubresource(0, 0, 0);
//		//HRESULT hr = m_poContext->Map(pNewTexture, subresource, D3D11_MAP::D3D11_MAP_READ, 0, &resource);
//		HRESULT hr = _poDeviceContext->Map(pStagingBackBuffer, subresource, D3D11_MAP::D3D11_MAP_READ, 0, &resource);
//
//		//resource.pData; // TEXTURE DATA IS HERE
//
//		const int pitch = width << 2;
//
//		unsigned char* source = static_cast<unsigned char*>(resource.pData);
//		vector<unsigned char> vecCaptured(height * (width * 4));
//
//		int index = 0;
//		for (int i = 0; i < height; ++i)
//		{
//			memcpy(&vecCaptured[index], source, width * 4);
//			source += resource.RowPitch;
//			index += width * 4;
//		}
//
//		_poDeviceContext->Unmap(pStagingBackBuffer, 0);
//
//		/*	m_captureSize = size;
//			m_captureWidth = width;
//			m_captureHeight = height;*/
//
//		time_t rawtime;
//
//		struct tm timeinfo;
//		char buffer[80];
//
//		time(&rawtime);
//		localtime_s(&timeinfo, &rawtime);
//
//		strftime(buffer, sizeof(buffer), "%d_%m_%Y_%H_%M_%S", &timeinfo);
//		std::string strTime(buffer);
//
//		/*QString qstrExePath = QCoreApplication::applicationDirPath() + "/screen_shots/";
//		QDir qDir;
//		qDir.mkdir(qstrExePath);*/
//
//		/*string strFíleName = qstrExePath.toStdString() +
//			_strPrefix + "RMEditor_Screenshot" + strTime + ".PNG";*/
//
//		int enccode_result = lodepng::encode(_strFileName,
//			vecCaptured, width, height, LodePNGColorType::LCT_RGBA, 8U);
//
//		string error = lodepng_error_text(enccode_result);
//
//		//return true;
//
//		//pBackBuffer->Release();
//		pStagingBackBuffer->Release();
//	}
//
//	return false;
//}
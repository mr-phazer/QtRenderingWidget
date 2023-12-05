#pragma once
//-------------------------------------------------------------------------------

/// RenderTarget.h

#include <memory>
// Direct3d 11 Headers
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include "..\..\..\DirectXTK\Inc\SimpleMath.h"
#include <wrl\client.h>
//nclude "BlendState.h"
//#include "Scene.h"
#include "..\..\..\DirectXTK\Inc\ScreenGrab.h"

// author headers
//#include "Scene.h"

//#include "Shader.h"
//#include "DXMeshBuffer.h"

//using namespace Microsoft::WRL;

struct RectFLoat
{
	// only set dim
	RectFLoat(float _width, float _height) :
		m_x(0.0f), m_y(0.0f), m_width(_width), m_height(_height)
	{};

	// init whole rect
	RectFLoat(float _x, float _y, float _width, float _height) :
		m_x(_x), m_y(_y), m_width(_width), m_height(_height)
	{};

	float x() const {
		return m_x; 
	}

	float y() const {
		return m_y; 
	}

	float width() const {
		return m_width; 
	}

	float height() const {
		return m_height; 
	}

private:
	float m_x, m_y, m_width, m_height;
};



//struct StringId
//{	
//	StringId(const string& _str)
//	{
//		srtingValue = _str;
//		intValue = internString(_str);
//	}
//
//	// get has value for string
//	static uint64_t internString(const string& _str)
//	{		
//		uint64_t hashValue = std::hash<std::string> {}(_str);
//
//		auto it = sm_mapStringIdToValue.find(_str);
//
//		if (it == sm_mapStringIdToValue.end())
//		{
//			sm_mapStringIdToValue[_str] = hashValue;
//		}
//
//		return hashValue;
//	}
//	
//	/*template<typename TYPE>
//	TYPE getValue(const string& _key)
//	{
//		return reinterpret_cast<TYPE>(getValueFromMap(_Key));
//	}
//
//	uint64_t _getValueFromMap(const string& _Key) { return 0;};*/
//
//	static std::unordered_map<string, uint64_t> sm_mapStringIdToValue;
//public:
//	string srtingValue = "";
//	uint64_t intValue = 0;
//};
//
//std::unordered_map<string, uint64_t> StringId::sm_mapStringIdToValue;
//

//bool operator==(const StringId& _left, const StringId& _right)
//{
//	return false;
//}


namespace Rldx
{
	class IDxTextureView
	{
	public:
		virtual ID3D11RenderTargetView* getRenderTargetView() = 0;
		virtual ID3D11ShaderResourceView* getGetShaderResourceView() = 0;
		virtual ID3D11Texture2D* getTexture() = 0;

		operator ID3D11RenderTargetView* () {
			return getRenderTargetView();
		};
		operator ID3D11ShaderResourceView* () {
			return getGetShaderResourceView();
		}
		operator ID3D11Texture2D* () {
			return getTexture();
		}
	};

	
	class DxTextureView : public IDxTextureView
	{
	protected:
		DirectX::XMFLOAT4 getPixelFromDepthBuffer(ID3D11DeviceContext* _poDeviceContext);

	public:
		typedef std::shared_ptr<DxTextureView> sptrTextureView;

		bool saveToDisk(ID3D11DeviceContext* _poDeviceContext, const string& _strFileName);

		DxTextureView() {
			sm_NextId = sm_ObjectCounter++; // TODO: CHANGE_MAYBE: why both next_id AND objectcounter statics, would ObjectCounter, not be enough
			m_id = sm_NextId;
		};

		static std::shared_ptr<DxTextureView> create(Scene* _poScene, bool _bCreateDepth = false, UINT _width = 1024, UINT _height = 1024);

		UINT getWidth() const {
			return m_textureWidth;
		};
		UINT getHeight() const {
			return m_textureHeight;
		};

		void SetToActiveTargetView(ID3D11DeviceContext* deviceContext)
		{			
			auto pRTV = getRenderTargetView();
			ID3D11RenderTargetView* const ppRTV[] = { pRTV };
			deviceContext->OMSetRenderTargets(1, ppRTV, NULL);
			
		}

//void renderQuadToTexture(rendutil::Scene* _poScene, rendutil::DxTextureView* _poRenderTarget);

// renderQuad without setting a target
		void renderQuad(Rldx::Scene* _poScene, float _width = 1.0f, float _height = 1.0f);
		void renderQuadToTexture(Rldx::Scene* _poScene, const DxTextureView* _poDest, float _width = 1.0f, float _height = 1.0f);
		//		void renderTo(rendutil::Scene* _poScene, const DxTextureView* _poDest, float _width = 1.0f, float _height = 1.0f);

		void copyTo_UsingCustomShader(Rldx::Scene* _poScene, DxTextureView* _poDest, Rldx::Shader* _poShader, bool _bScaleUV = false, float _width = 2.0f, float _height = 2.0f); // Render to texture with specified shader

		void renderTo(Rldx::Scene* _poScene, DxTextureView* _poDest, bool _bScaleUV = false, float _width = 2.0f, float _height = 2.0f); // Rendder to texture with standard copy shader

		// TODO: make these "copy", and let the down/upsampling happing automatically based on the resolution of the textures
		void downSample(Scene* _poScene, DxTextureView* _poDest);
		void upSample(Rldx::Scene* _poScene, DxTextureView* _poDest, Rldx::Shader* _poShader = nullptr);
		void upSample_Cam(Rldx::Scene* _poScene, DxTextureView* _poDest, Rldx::Shader* _poShader = nullptr);


		void copyToBuffer(Scene* _poScene, DxTextureView* _poDest, Shader* _poShader = nullptr);
		
		void copyToBufferPosDim(Scene* _poScene, DxTextureView* _poDest, const RectFLoat& rectF, Shader* _poShader = nullptr);


		ID3D11DepthStencilView* GetDepthStencilView()
		{
			return m_poDepthStencilView.Get();
		}
		
		ID3D11ShaderResourceView* GetDepthShaderResourceView()
		{
			return m_cpoDepthShaderResourceView.Get();
		}
		/*void copyTo(rendutil::Scene* _poScene, DxTextureView* _poDest,
			int _destPosX, int _destPosY, int _pDestWdith = -1, int _pDestHeight = -1);*/

		/// <summary>
		/// {NOT IMPLEMENTED YET}
		/// Renders one DxTextureView into another taking care of scaling, etc
		/// </summary>
		/// <param name="_poDevice"></param>
		/// <param name="_poDeviceContext"></param>
		/// <param name="_poDestTexture"></param>
		static void copyWithAutoScaing(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, const DxTextureView* _poSrcTexture, DxTextureView* _poDestTexture);


		void drawDepthBuffer(Rldx::Scene* _poScene, DxTextureView* _poDest);

		void resolveDepthBuffer(Rldx::Scene* _poScene);

		static void unbindShaderResourceViews(Rldx::Scene* poScene, UINT _slot = 0, UINT _count = 1);
		static void unbindRenderTargetView(Rldx::Scene* poScene, UINT _slot = 0, UINT _count = 1);

		void setActiveDestRenderTargetAndDSV(Rldx::Scene* _poScene) const;
		void setActiveDestRenderTargetAndDSVNULL(Rldx::Scene* _poScene) const;

		void setActiveDestRenderTargetAndDSV_RAW(Rldx::Scene* _poScene) const;
		void setActiveDestRenderTargetOnly_RAW(Rldx::Scene* _poScene) const;

		void setActiveDestRenderTarget_DSV_NULL(Rldx::Scene* _poScene) const;
		void setActiveDestRenderTarget_DSV_NULL_RAW(Rldx::Scene* _poScene) const;

		void setActiveDSVOnly_RAW(Rldx::Scene* _poScene);
		void setActiveDSVOnly(Rldx::Scene* _poScene);

		void clearPixelsAndDepth(Rldx::Scene* _poScene, sm::Color vColor = { 0,0,0,1 });
		void clearDepthOnly(Rldx::Scene* _poScene);

		void clearAndDepth_RAW(Rldx::Scene* _poScene, sm::Color vColor = { 0,0,0,1 });
		void clearDepthOnly_RAW(Rldx::Scene* _poScene);
		//void clearAndDepth_RAW(rendutil::Scene* _poScene, SimpleMath::Color vColor = { 0,0,0,1 });

		void clearPixels(Rldx::Scene* _poScene, sm::Color vColor = { 0,0,0,1 });

		void clearPixels(ID3D11DeviceContext*, sm::Color vColor = { 0,0,0,1 });

		//RAW(rendutil::Scene* _poScene, SimpleMath::Color vColor = { 0,0,0,1 });

		//ID3D11Texture2D* getTexture() { return m_cpoTexture.Get(); };
		//ID3D11RenderTargetView* getRenderTargetView() { return m_cpoRenderTargetView.Get(); };
		//ID3D11ShaderResourceView* getShaderResourceView() { return m_cpoShaderResourceView.Get(); };

		void setTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> _cpoTexture) {
			m_cpoTexture = _cpoTexture;
		};
		void setRenderTargetView(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _cpoRenderTargetView) {
			m_cpoRenderTargetView = _cpoRenderTargetView;
		};
		void setShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _cpoShaderResourceView) {
			m_cpoShaderResourceView = _cpoShaderResourceView;
		};

		void setRenderTargetView_RAW(ID3D11RenderTargetView* _poRenderTargetView);
		void setShaderResourceView_RAW(ID3D11ShaderResourceView* _poShaderResourceView);
		void setDepthStencilView_RAW(ID3D11DepthStencilView* _poDepthStenticlView);

		virtual ID3D11RenderTargetView* getRenderTargetView()  override {
			return m_cpoRenderTargetView.Get();
		};
		virtual ID3D11ShaderResourceView* getGetShaderResourceView()  override {
			return m_cpoShaderResourceView.Get();
		};
		virtual ID3D11Texture2D* getTexture()  override {
			return m_cpoTexture.Get();
		};

		virtual ID3D11RenderTargetView** getAddressOfRenderTargetView() /* override*/ {
			return m_cpoRenderTargetView.GetAddressOf();
		};
		virtual ID3D11ShaderResourceView** getAddressOfShaderResourceView()  /*override*/ {
			return m_cpoShaderResourceView.GetAddressOf();
		};
		virtual ID3D11Texture2D** getGetAddressOfTexture()  /*override*/ {
			return m_cpoTexture.GetAddressOf();
		};

		virtual Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getRenderTargetViewCPO()  /*override */ {
			return m_cpoRenderTargetView;
		};
		virtual Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getGetShaderResourceViewCPO()  /*override*/ {
			return m_cpoShaderResourceView;
		};
		virtual Microsoft::WRL::ComPtr<ID3D11Texture2D>& getGetTexturePO()  /*override*/ {
			return m_cpoTexture;
		};

//private:

		void init_views(
			const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& _cpoRTV = Microsoft::WRL::ComPtr< ID3D11RenderTargetView>(nullptr),
			const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& _cpoDepthView = Microsoft::WRL::ComPtr< ID3D11DepthStencilView>(nullptr),
			const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthTexture = Microsoft::WRL::ComPtr< ID3D11Texture2D>(nullptr),
			const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthSRV = Microsoft::WRL::ComPtr< ID3D11Texture2D>(nullptr),
			const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthView_ReadOnly = Microsoft::WRL::ComPtr< ID3D11Texture2D>(nullptr)
		);

		bool init_ms(Rldx::Scene* _poScene, UINT width, UINT height,
			const char* _szObjectName = nullptr);

		virtual bool reset(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, UINT width, UINT height);

		bool resetToMatch(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, const DxTextureView& src)
		{
			// don't do anything, if same size
			if (src.getWidth() == this->getWidth() && src.getHeight() == this->getHeight())
			{
				return true;
			}

			return reset(_poDevice, _poDeviceContext, src.getWidth(), src.getHeight());
		}


		bool init(
			ID3D11Device* _poDevice,
			ID3D11DeviceContext* _poDeviceContext,
			UINT width,
			UINT height,
			DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
			UINT _uiSampleCount = 1,
			const char* _szObjectName = nullptr
		);

		bool _initTextureBuffers(
			ID3D11Device* _poDevice,
			UINT width = 1024,
			UINT height = 1024,
			DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
			UINT _uiSampleCount = 1,
			const char* _szObjectName = nullptr
		);

		bool initDepthStencilView(
			ID3D11Device* _poDevice,
			UINT width,
			UINT height,
			DXGI_FORMAT Format = DXGI_FORMAT_R32_FLOAT,
			UINT _uiSampleCount = 1,
			const char* _szObjectName = nullptr
		);

		//	bool initWithFrameBuffer(rendutil::Scene* _poScene, UINT width, UINT height);

		bool init_ms(
			Scene* _poScene,
			UINT Width,
			UINT Height,
			DXGI_FORMAT Format,
			DXGI_SAMPLE_DESC SampleDesc = { 0, 1 },
			D3D11_USAGE Usage = D3D11_USAGE_DEFAULT,
			UINT BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE

		);

		/*bool init_ms(
			Scene* _poScene,
			UINT Width,
			UINT Height,
			DXGI_FORMAT Format,
			DXGI_SAMPLE_DESC SampleDesc = { 0, 1 },
			D3D11_USAGE Usage = D3D11_USAGE_DEFAULT,
			UINT BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE

		);*/

		//bool init(
		//	Scene* _poScene,
		//	UINT Width,
		//	UINT Height,
		//	DXGI_FORMAT Format,
		//	DXGI_SAMPLE_DESC SampleDesc = { 0, 1 },
		//	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT,
		//	UINT BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE

		//);

		//private:

		HRESULT init_impl(
			Scene* _poScene,
			// D3D11_TEXTURE2D_DESC content
			UINT Width,
			UINT Height,
			UINT MipLevels,
			UINT ArraySize,
			DXGI_FORMAT Format,
			DXGI_SAMPLE_DESC SampleDesc,
			D3D11_USAGE Usage,
			UINT BindFlags,
			UINT CPUAccessFlags,
			UINT MiscFlags
		);

		bool createQuad(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext);
		bool updateQuad(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, const RectFLoat& rectF);


		bool resetQuad(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, const RectFLoat& rectF);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cpoTexture;
	private:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_cpoRenderTargetView;
	public:

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cpoShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cpoDepthStencilTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_poDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_cpoDepthStencilView_ReadOnly;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cpoDepthShaderResourceView;

		ID3D11Texture2D* m_cpoTexture_RAW = nullptr;
		ID3D11RenderTargetView* m_poRenderTargetView_RAW = nullptr;
		ID3D11ShaderResourceView* m_cpoShaderResourceView_RAW = nullptr;;

		ID3D11Texture2D* m_pDepthStencilTexture_RAW_External = nullptr;;
		ID3D11DepthStencilView* m_pDepthStencilView_RAW_External = nullptr;;
		ID3D11DepthStencilView* m_pDepthStencilView_ReadOnly_RAW_External = nullptr;;
		ID3D11ShaderResourceView* m_pDepthShaderResourceView_RAW_External = nullptr;;

		/*ID3D11Texture2D* m_poDepthStencilTexture_RAW = nullptr;
		ID3D11DepthStencilView* m_poDepthStencilView = nullptr;;*/

		ID3D11DepthStencilState* m_poDepthStencilState_On = nullptr;
		ID3D11DepthStencilState* m_poDepthStencilState_Off = nullptr;

		//ID3D11Texture2D* m_poTexture_RAW = nullptr;
		//ID3D11RenderTargetView* m_poRenderTargetView_RAW = nullptr;
		//ID3D11ShaderResourceView* m_poShaderResourceView_RAW = nullptr;
		////ID3D11DepthStencilView* m_poDepthStencilView_RAW = nullptr;
		//ID3D11ShaderResourceView* m_poDepthShaderResourceView_RAW = nullptr;

		D3D11_TEXTURE2D_DESC m_textureDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC m_shaderResourceViewDesc;

		Shader::sptrShader m_spoQuadShader;

		spDXMeshBuffer m_spoQuadMesh;

		BlendState m_oBlendState;

	public:
		bool m_useExternalBuffers = false;

		float m_textureWidth = 1.0;
		float m_textureHeight = 1.0;

		float m_quadWidth = 1.0;
		float m_quadHeight = 1.0;

		float m_v_div = 1.0f;
		float m_u_div = 1.0f;


		size_t m_id = 0;
		static size_t sm_ObjectCounter;
		static size_t sm_NextId;
	};


#if 0
	struct G_Buffer
	{
		void init(ID3D11Device* _poDevice)
		{
			spo_Position_Buffer = make_shared < DxTextureView>();
			spo_Normal_Buffer = make_shared < DxTextureView>();

			reset(_poDevice, 1024, 1024);

			m_G_Buffer_Shader = rendutil::Shader::create(
				_poDevice,
				layout_Weighted4,
				_countof(layout_Weighted4),
				LR"(VS_Default.cso)",
				LR"(ps_defer_position.cso)");
		}

		void setasRenderTargets(rendutil::Scene* _poScene);

		void reset(ID3D11Device* _poDevice, UINT width, UINT height)
		{
			spo_Position_Buffer->init(, width, height);

			/*TextureView* view;
			function<void()> func = [this, view]()
			{
				view->init(nullptr);
			};*/
		};

		/*TextureView::sptrTextureView getBuffer(size_t _index);
		TextureView::sptrTextureView getBuffer(const string& _strId);

		map<string, size_t> mapStringIdToBufferIndex;
		map<string, size_t> mapStringIdToBufferIndex;
		vector<TextureView::sptrTextureView> vecBuffers;*/

		TextureView::sptrTextureView spo_Position_Buffer;
		TextureView::sptrTextureView spo_Normal_Buffer;

		Shader::sptrShader m_G_Buffer_Shader;
	};

#endif // 0


	// 
	class DXDoubleTextureView : public DxTextureView
	{
	public:
		typedef shared_ptr<DXDoubleTextureView> spDXDoubleTextureView;
		static spDXDoubleTextureView create(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, UINT width, UINT heigh, DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT);
		

		virtual bool reset(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, UINT width, UINT height) override
		{
			DxTextureView::reset(_poDevice, _poDeviceContext, width, height);
			m_tempBuffer->reset(_poDevice, _poDeviceContext, width, height);
		}

		DxTextureView* extra() {
			return m_tempBuffer.get();
		};

		DxTextureView::sptrTextureView m_tempBuffer; 
	};

}
//#pragma once
////-------------------------------------------------------------------------------
//
///// RenderTarget.h
//
//// Direct3d 11 Headers
//#include <d3d11.h>
//#include <d3dcompiler.h>
//#include <DirectXMath.h>
//#include <DirectXColors.h>
//#include <DirectXMath.h>
//#include "..\DirectXTK\Inc\SimpleMath.h"
//#include <wrl\client.h>
//#include "BlendState.h"
////#include "Scene.h"
//#include "..\DirectXTK\Inc\ScreenGrab.h"
//
//// author headers
////#include "Scene.h"
//
//#include "Shader.h"
//#include "DXMeshBuffer.h"
//
////using namespace Microsoft::WRL;
//
//
//
//
//namespace rendutil
//{
//	class ITextureView
//	{
//	public:
//		virtual ID3D11RenderTargetView* getRenderTargetView() = 0;
//		virtual ID3D11ShaderResourceView* getGetShaderResourceView() = 0;
//		virtual ID3D11Texture2D* getTexture() = 0;
//
//		operator ID3D11RenderTargetView* () { return getRenderTargetView(); };
//		operator ID3D11ShaderResourceView* () { return getGetShaderResourceView(); }
//		operator ID3D11Texture2D* () { return getTexture(); }
//	};
//
//	class Scene;
//	class DxTextureView : public ITextureView
//	{
//	protected:
//
//		XMFLOAT4 getPixelFromDepthBuffer(ID3D11DeviceContext* _poDeviceContext)
//		{
//			//desc.SampleDesc.Count = 1;
//			//desc.SampleDesc.Quality = 0;
//			//desc.BindFlags = 0;
//			//desc.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;
//			//desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
//			//desc.Usage = D3D11_USAGE_STAGING;
//
//
//			//hr = d3dDevice->CreateTexture2D(&desc, nullptr, &pStaging);
//			//if (FAILED(hr))
//			//	return hr;
//
//
//			//D3D11_BOX srcBox;
//			//srcBox.left = 10;
//			//srcBox.right = srcBox.left + 1;
//			//srcBox.top = 10;
//			//srcBox.bottom = srcBox.top + 1;
//			//srcBox.front = 0;
//			//srcBox.back = 1;
//
//			//_poDeviceContext->CopySubresourceRegion(p1x1StagingTexture, 0, 0, 0, 0, m_cpoDepthStencilTexture.Get(), 0, &srcBox);
//
//			//D3D11_MAPPED_SUBRESOURCE msr;
//			//_poDeviceContext->Map(p1x1StagingTexture, 0, D3D11_MAP_READ, 0, &msr);
//			//BYTE *pixel = msr.pData;
//			//// copy data
//			//_poDeviceContexts->Unmap(p1x1StagingTexture, 0);
//		}
//
//	public:
//		typedef shared_ptr<TextureView> sptrTextureView;
//
//		bool saveToDisk(ID3D11DeviceContext* _poDeviceContext, const string& _strFileName);
//
//		TextureView() { sm_NextId = sm_ObjectCounter++; };
//
//		static shared_ptr<TextureView> create(Scene* _poScene, bool _bCreateDepth = false, UINT _width = 1024, UINT _height = 1024);
//
//		UINT getWidth() const { return m_textureWidth; };
//		UINT getHeight() const { return m_textureHeight; };
//
//		//void renderQuadToTexture(rendutil::Scene* _poScene, rendutil::DxTextureView* _poRenderTarget);
//
//		// renderQuad without setting a target
//		void renderQuad(rendutil::Scene* _poScene, float _width = 1.0f, float _height = 1.0f);
//		void renderQuadToTexture(rendutil::Scene* _poScene, const DxTextureView* _poDest, float _width = 1.0f, float _height = 1.0f);
//		//		void renderTo(rendutil::Scene* _poScene, const DxTextureView* _poDest, float _width = 1.0f, float _height = 1.0f);
//
//		void copyTo(rendutil::Scene* _poScene, DxTextureView* _poDest, rendutil::Shader* _poShader, bool _bScaleUV = false, float _width = 2.0f, float _height = 2.0f); // Render to texture with specified shader
//
//		void renderTo(rendutil::Scene* _poScene, DxTextureView* _poDest, bool _bScaleUV = false, float _width = 2.0f, float _height = 2.0f); // Rendder to texture with standard copy shader
//
//		void downSample(Scene* _poScene, DxTextureView* _poDest);
//		void upSample(rendutil::Scene* _poScene, DxTextureView* _poDest, rendutil::Shader* _poShader = nullptr);
//		void upSample_Cam(rendutil::Scene* _poScene, DxTextureView* _poDest, rendutil::Shader* _poShader = nullptr);
//
//		void drawDepthBuffer(rendutil::Scene* _poScene, DxTextureView* _poDest);
//
//		void resolveDepthBuffer(rendutil::Scene* _poScene);
//
//		static void unbindShaderResourceViews(rendutil::Scene* poScene, UINT _slot = 0, UINT _count = 1);
//		static void unbindRenderTargetView(rendutil::Scene* poScene, UINT _slot = 0, UINT _count = 1);
//
//		void setActiveDestRenderTargetAndDSV(rendutil::Scene* _poScene) const;
//		void setActiveDestRenderTargetAndDSVNULL(rendutil::Scene* _poScene) const;
//
//		void setActiveDestRenderTargetAndDSV_RAW(rendutil::Scene* _poScene) const;
//		void setActiveDestRenderTargetOnly_RAW(rendutil::Scene* _poScene) const;
//
//		void setActiveDestRenderTarget_DSV_NULL(rendutil::Scene* _poScene) const;
//		void setActiveDestRenderTarget_DSV_NULL_RAW(rendutil::Scene* _poScene) const;
//
//		void setActiveDSVOnly_RAW(rendutil::Scene* _poScene);
//		void setActiveDSVOnly(rendutil::Scene* _poScene);
//
//		void clearPixelsAndDepth(rendutil::Scene* _poScene, SimpleMath::Color vColor = { 0,0,0,1 });
//		void clearDepthOnly(rendutil::Scene* _poScene);
//
//		void clearAndDepth_RAW(rendutil::Scene* _poScene, SimpleMath::Color vColor = { 0,0,0,1 });
//		void clearDepthOnly_RAW(rendutil::Scene* _poScene);
//		//void clearAndDepth_RAW(rendutil::Scene* _poScene, SimpleMath::Color vColor = { 0,0,0,1 });
//
//		void clearPixels(rendutil::Scene* _poScene, SimpleMath::Color vColor = { 0,0,0,1 });
//
//		void clearPixels(ID3D11DeviceContext*, SimpleMath::Color vColor = { 0,0,0,1 });
//
//		//RAW(rendutil::Scene* _poScene, SimpleMath::Color vColor = { 0,0,0,1 });
//
//		//ID3D11Texture2D* getTexture() { return m_cpoTexture.Get(); };
//		//ID3D11RenderTargetView* getRenderTargetView() { return m_cpoRenderTargetView.Get(); };
//		//ID3D11ShaderResourceView* getShaderResourceView() { return m_cpoShaderResourceView.Get(); };
//
//		void setTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> _cpoTexture) { m_cpoTexture = _cpoTexture; };
//		void setRenderTargetView(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _cpoRenderTargetView) { m_cpoRenderTargetView = _cpoRenderTargetView; };
//		void setShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _cpoShaderResourceView) { m_cpoShaderResourceView = _cpoShaderResourceView; };
//
//		void setRenderTargetView_RAW(ID3D11RenderTargetView* _poRenderTargetView);
//		void setShaderResourceView_RAW(ID3D11ShaderResourceView* _poShaderResourceView);
//		void setDepthStencilView_RAW(ID3D11DepthStencilView* _poDepthStenticlView);
//
//		virtual ID3D11RenderTargetView* getRenderTargetView()  override { return m_cpoRenderTargetView.Get(); };
//		virtual ID3D11ShaderResourceView* getGetShaderResourceView()  override { return m_cpoShaderResourceView.Get(); };
//		virtual ID3D11Texture2D* getTexture()  override { return m_cpoTexture.Get(); };
//
//		virtual ID3D11RenderTargetView** getAddressOfOfTargetView() /* override*/ { return m_cpoRenderTargetView.GetAddressOf(); };
//		virtual ID3D11ShaderResourceView** getAddressOfOfShaderResourceView()  /*override*/ { return m_cpoShaderResourceView.GetAddressOf(); };
//		virtual ID3D11Texture2D** getGetAddressOfTexture()  /*override*/ { return m_cpoTexture.GetAddressOf(); };
//
//		virtual Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getRenderTargetViewCPO()  /*override */ { return m_cpoRenderTargetView; };
//		virtual Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getGetShaderResourceViewCPO()  /*override*/ { return m_cpoShaderResourceView; };
//		virtual Microsoft::WRL::ComPtr<ID3D11Texture2D> getGetTexturePO()  /*override*/ { return m_cpoTexture; };
//
//		//private:
//
//		void init_views(
//			const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& _cpoRTV = Microsoft::WRL::ComPtr< ID3D11RenderTargetView>(nullptr),
//			const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& _cpoDepthView = Microsoft::WRL::ComPtr< ID3D11DepthStencilView>(nullptr),
//			const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthTexture = Microsoft::WRL::ComPtr< ID3D11Texture2D>(nullptr),
//			const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthSRV = Microsoft::WRL::ComPtr< ID3D11Texture2D>(nullptr),
//			const Microsoft::WRL::ComPtr<ID3D11Texture2D>& _cpoDepthView_ReadOnly = Microsoft::WRL::ComPtr< ID3D11Texture2D>(nullptr)
//		);
//
//		bool init_ms(rendutil::Scene* _poScene, UINT width, UINT height,
//			const char* _szObjectName = nullptr);
//
//		bool init(
//			Scene* _poScene,
//			UINT width,
//			UINT height,
//			DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
//			UINT _uiSampleCount = 1,
//			const char* _szObjectName = nullptr
//		);
//
//		bool init(
//			ID3D11Device* _poDevice,
//			UINT width = 1024,
//			UINT height = 1024,
//			DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
//			UINT _uiSampleCount = 1,
//			const char* _szObjectName = nullptr
//		);
//
//		bool initDepthStencilView(
//			Scene* _poScene,
//			UINT width,
//			UINT height,
//			DXGI_FORMAT Format = DXGI_FORMAT_R32_FLOAT,
//			UINT _uiSampleCount = 1,
//			const char* _szObjectName = nullptr
//		);
//
//		//	bool initWithFrameBuffer(rendutil::Scene* _poScene, UINT width, UINT height);
//
//		bool init_ms(
//			Scene* _poScene,
//			UINT Width,
//			UINT Height,
//			DXGI_FORMAT Format,
//			DXGI_SAMPLE_DESC SampleDesc = { 0, 1 },
//			D3D11_USAGE Usage = D3D11_USAGE_DEFAULT,
//			UINT BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
//
//		);
//
//		/*bool init_ms(
//			Scene* _poScene,
//			UINT Width,
//			UINT Height,
//			DXGI_FORMAT Format,
//			DXGI_SAMPLE_DESC SampleDesc = { 0, 1 },
//			D3D11_USAGE Usage = D3D11_USAGE_DEFAULT,
//			UINT BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
//
//		);*/
//
//		//bool init(
//		//	Scene* _poScene,
//		//	UINT Width,
//		//	UINT Height,
//		//	DXGI_FORMAT Format,
//		//	DXGI_SAMPLE_DESC SampleDesc = { 0, 1 },
//		//	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT,
//		//	UINT BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
//
//		//);
//
//		//private:
//
//		HRESULT init_impl(
//			Scene* _poScene,
//			// D3D11_TEXTURE2D_DESC content
//			UINT Width,
//			UINT Height,
//			UINT MipLevels,
//			UINT ArraySize,
//			DXGI_FORMAT Format,
//			DXGI_SAMPLE_DESC SampleDesc,
//			D3D11_USAGE Usage,
//			UINT BindFlags,
//			UINT CPUAccessFlags,
//			UINT MiscFlags
//		);
//
//		bool resetQuad(Scene* _poScene, UINT _width, UINT _height);
//
//		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cpoTexture;
//		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_cpoRenderTargetView;
//		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cpoShaderResourceView;
//
//		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cpoDepthStencilTexture;
//		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_poDepthStencilView;
//		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_cpoDepthStencilView_ReadOnly;
//		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cpoDepthShaderResourceView;
//
//		ID3D11Texture2D* m_cpoTexture_RAW = nullptr;
//		ID3D11RenderTargetView* m_poRenderTargetView_RAW = nullptr;
//		ID3D11ShaderResourceView* m_cpoShaderResourceView_RAW = nullptr;;
//
//		ID3D11Texture2D* m_cpoDepthStencilTexture_RAW = nullptr;;
//		ID3D11DepthStencilView* m_cpoDepthStencilView_RAW = nullptr;;
//		ID3D11DepthStencilView* m_cpoDepthStencilView_ReadOnly_RAW = nullptr;;
//		ID3D11ShaderResourceView* m_cpoDepthShaderResourceView_RAW = nullptr;;
//
//		/*ID3D11Texture2D* m_poDepthStencilTexture_RAW = nullptr;
//		ID3D11DepthStencilView* m_poDepthStencilView = nullptr;;*/
//
//		ID3D11DepthStencilState* m_poDepthStencilState_On = nullptr;
//		ID3D11DepthStencilState* m_poDepthStencilState_Off = nullptr;
//
//		//ID3D11Texture2D* m_poTexture_RAW = nullptr;
//		//ID3D11RenderTargetView* m_poRenderTargetView_RAW = nullptr;
//		//ID3D11ShaderResourceView* m_poShaderResourceView_RAW = nullptr;
//		////ID3D11DepthStencilView* m_poDepthStencilView_RAW = nullptr;
//		//ID3D11ShaderResourceView* m_poDepthShaderResourceView_RAW = nullptr;
//
//		D3D11_TEXTURE2D_DESC m_textureDesc;
//
//		Shader::sptrShader spoQuadShader;
//
//		shared_ptr<DXMeshBuffer> spoQuadMesh;
//
//		BlendState m_oBlendState;
//
//	public:
//		UINT m_textureWidth = 1.0;
//		UINT m_textureHeight = 1.0;
//
//		float m_quadWidth = 1.0;
//		float m_quadHeight = 1.0;
//
//		float m_v_div = 1.0f;
//		float m_u_div = 1.0f;
//
//		static size_t sm_ObjectCounter;
//		static size_t sm_NextId;
//	};
//
//	struct G_Buffer
//	{
//		void init(ID3D11Device* _poDevice)
//		{
//			spo_Position_Buffer = make_shared < DxTextureView>();
//			spo_Normal_Buffer = make_shared < DxTextureView>();
//
//			reset(_poDevice, 1024, 1024);
//
//			m_G_Buffer_Shader = rendutil::Shader::create(
//				_poDevice,
//				layout_Weighted4,
//				_countof(layout_Weighted4),
//				LR"(VS_Default.cso)",
//				LR"(ps_defer_position.cso)");
//		}
//
//		void setasRenderTargets(rendutil::Scene* _poScene);
//
//		void reset(ID3D11Device* _poDevice, UINT width, UINT height)
//		{
//			spo_Position_Buffer->init(_poDevice, width, height);
//
//			/*TextureView* view;
//			function<void()> func = [this, view]()
//			{
//				view->init(nullptr);
//			};*/
//		};
//
//		/*TextureView::sptrTextureView getBuffer(size_t _index);
//		TextureView::sptrTextureView getBuffer(const string& _strId);
//
//		map<string, size_t> mapStringIdToBufferIndex;
//		map<string, size_t> mapStringIdToBufferIndex;
//		vector<TextureView::sptrTextureView> vecBuffers;*/
//
//		TextureView::sptrTextureView spo_Position_Buffer;
//		TextureView::sptrTextureView spo_Normal_Buffer;
//
//		Shader::sptrShader m_G_Buffer_Shader;
//	};
//};
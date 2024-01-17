#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include "..\DirectXTK\Inc\SimpleMath.h"
#include <DirectXMath.h>
#include <map>
#include <string>
//#include "TextOutDebug.h"
//#include "..\SystemLib\tools.h"
//#include "Scene/DxSceneGraph/NodeTransform/NodeTransform.h"

#define MOUSE_LEFT_BUTTON   0x01
#define MOUSE_MIDDLE_BUTTON 0x02
#define MOUSE_RIGHT_BUTTON  0x04
#define MOUSE_WHEEL         0x08

enum class Key : uint32_t
{
	Up,
	Down,
	Left,
	Right,
	Forward,
	Backward,
};

// associate input key code with action key
//std::map<WPARAM, Key> mapKeyMap;
namespace rldx
{
	class DxCameraOrbital
	{

	public:		

		//MyOrbitalCamera(float yaw = 0.8f, float pitch = -0.32f, float currentZoom = 1)
		DxCameraOrbital(float yaw = 0.0, float pitch = 0.0, float currentZoom = 0.1)
		{
			m_geometryData.m_zoom = currentZoom;
			m_geometryData.fYaw = yaw;
			m_geometryData.fPitch = pitch;
			//_lookAt = lookAt;

			SetRadius();
			//   UpdateOrder = (int)ComponentUpdateOrderEnum.Camera;
		}


		sm::Vector3 WorldSpaceToScreenSpace(const sm::Vector3& worldPosition, const sm::Matrix& worldMatrix)
		{
			//poView->m_poD3DWidget->scene()->getDeviceContext()->RSGetViewports(&viewPortCount, &dxViewPort);

			sm::Viewport viewPort(0, 0, m_width, m_height);

			auto screenSpacePosition = viewPort.Project(worldPosition, GetProjMatrix(), GetViewMatrix(), worldMatrix);

			//auto viewSpacePosition = sm::Vector3::Transform(worldPosition, GetViewMatrix());  // -> to view space ?
			//auto clipSpacePosition = sm::Vector3::Transform(viewSpacePosition, GetProjMatrix()); //  -> to clip space ?

			return screenSpacePosition;
		}

		void save_to_disk(const std::wstring& _path);

		float m_fDeAccelerate = 0.01f;

		//float m_fAspect;                        // Aspect ratio
		//float m_fNearPlane;                     // Near plane
		//float m_fFarPlane;                      // Far plane

		float m_fRotationScaler = 0.01f;         // Scaler for rotation
		float m_fMoveScaler = 0.01f;            // Scaler for movement


		struct GeometryData
		{
			DirectX::SimpleMath::Vector3 m_v3Origin = { 0,0,0 };
			sm::Vector3 v3NewPosition = { 0,0,0 };
			sm::Vector3 v3EyePosition;// = sm::Vector3(0.30336335f, 5.1672184f, 2.3106112f);
			sm::Vector3 v3Up = { 0, 1, 0 };

			float fYaw = -DirectX::XM_PIDIV4;; // -0.949992001;
			float fPitch = -DirectX::XM_PIDIV4;; // -0.410030514;
			float fRoll = 0.0f;

			float m_world_yaw = 0; // DirectX::XM_PIDIV4;;
			float m_world_pitch = 0; // -DirectX::XM_PIDIV4;
			float m_world_roll = 0;
			float m_zoom = 0.0f;

			sm::Vector3 v3LookAt = sm::Vector3(0.f, 0.f, 0.f);
			float fFieldOfView = DirectX::XM_PI / 4.f;
			float nearDistance = 0.01f;
			float farDistance = 100.f;

			float fRadius = 0.1;                        // Distance from the camera to model
			float fDefaultRadius = 1.0;                 // Distance from the camera to model
			float fMinRadius = 0.00001;                     // Min radius
			float fMaxRadius = 10;                     // Max radius

		} m_geometryData;

		bool m_bLHCoords = true;
		bool m_bAttachCameraToModel;
		bool m_bLimitPitch;
		bool m_bDragSinceLastUpdate;            // True if mouse drag has happened since last time MoveFrame is called.


		void MoveCameraRight(float amount);
	public:

		void RayCast(const DirectX::SimpleMath::Vector2& ptCursor, const DirectX::SimpleMath::Vector2& screenDims,
			DirectX::SimpleMath::Vector3& _ray,
			DirectX::SimpleMath::Vector3& _origin);


		//void RayCast(sm::Vector2 ptCursor, sm::Vector2 screenDims, sm::Vector3& _ray_origin_world, sm::Vector3& _ray_direction_world);



		void MoveCameraUp(float amount)
		{
			m_geometryData.v3LookAt.y += amount;
			//m_bIsViewMatrixDirty = true;
		}

		/// <summary>
		/// Moves the camera (lookAt) forward,
		/// as seen from the camera, while keeping the same height
		/// </summary>
	public:
		void MoveCameraForward(float amount);

		HRESULT HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		// Set projection window (pixels)
		void SetWindow(int width, int height);

		void SetEyePosition(const sm::Vector3& pos);
		void SetLookAt(const sm::Vector3& lookAt);

		void SetFieldOfView(float value);

		void SetViewParams(const sm::Vector3& pos, const sm::Vector3& lookAt)
		{
			SetEyePosition(pos);
			SetLookAt(lookAt);
		}

		const sm::Matrix& GetViewMatrix();
		void CalculateEyePostion_RotationMatrix();
		void CalculateEyePosition_Trigonometric();
		const sm::Matrix& GetWorldMatrix();

		sm::Vector3 GetEyePt() const;
		sm::Vector3 GetLookAt() const;

		DirectX::XMVECTOR ScreenToVector(float screenx, float screeny) const;

		void OnBegin(int x, int y);

		void OnMove(int x, int y);

		void OnWheel(float delta);

		void OnEnd();

		void RotateCamera();

		void rotateCamera_Yaw(float angle = (DirectX::XM_2PI) * 0.001);
		void rotateCamera_Pitch(float angle = (DirectX::XM_2PI) * 0.001);
		void rotateCameraD(float angle = (DirectX::XM_2PI) * 0.001);

		//void rotateCameraRight(float angle = (DirectX::XM_2PI) * 0.001);

		void moveCameraUp();
		void moveCameraDown();
		void moveCameraLeft();
		void moveCameraRight();
		void moveCameraForward();
		void moveCameraBackward();

		float& value1() {
			return m_value1;
		};
		float m_value1;

		sm::Matrix& GetProjMatrix();

		void UpdateVelocity(_In_ float fElapsedTime);

		bool IsMouseMButtonDown() {
			return MOUSE_R_BUTTON_DOWN || MOUSE_L_BUTTON_DOWN || MOUSE_M_BUTTON_DOWN;
		}

		//void SetProjParams(float fov, float fAspectRatio, float nearDistance, float farDistance, bool lhcoords = true);
		virtual void SetProjParams(_In_ float fFOV, _In_ float fAspect, _In_ float fNearPlane, _In_ float fFarPlane);
		//void SetScalers(float )

		void MoveFrame(float time);



		void MoveLookAt();

		void Zoom();

		POINT m_ptLastMousePosition = { 0,0 };            // Last absolute position of mouse cursor
		float		m_fFramesToSmoothMouseData = 2.0f;
		DirectX::XMFLOAT2 m_vMouseDelta = { 0,0 };

		void UpdateMouseDelta();

		int m_nRotateModelButtonMask;
		int m_nZoomButtonMask;
		int m_nRotateCameraButtonMask;

		void SetButtonMasks(_In_ int nRotateModelButtonMask = MOUSE_MIDDLE_BUTTON, _In_ int nZoomButtonMask = MOUSE_WHEEL,
			_In_ int nRotateCameraButtonMask = MOUSE_LEFT_BUTTON
			/*_In_ int nTranskateButtonMask */
		)
		{
			m_nRotateModelButtonMask = nRotateModelButtonMask;
			m_nZoomButtonMask = nZoomButtonMask;
			m_nRotateCameraButtonMask = nRotateCameraButtonMask;
		}

		void SetRadius(_In_ float fDefaultRadius = 0.5f, _In_ float fMinRadius = 1.0f, _In_ float fMaxRadius = FLT_MAX)
		{
			m_geometryData.fDefaultRadius = m_geometryData.fRadius = fDefaultRadius; 
			m_geometryData.fMinRadius = fMinRadius; 
			m_geometryData.fMaxRadius = fMaxRadius;
			m_bDragSinceLastUpdate = true;
		}

		/*void setWorldMatrixToControl(NodeTransform* _transformWorldExternal)
		{
			if (SHIFT_BUTTON_DOWN)
			{
				m_transformExternal = _transformWorldExternal;
			}
		}*/
		// TODO: cleanup!!
	private:
		//NodeTransform* m_transformExternal = nullptr;

		//DirectX::XMFLOAT4X4* m_destView = nullptr;

		bool m_bDrag = false;
		sm::Matrix m_mView = sm::Matrix::Identity;
		sm::Matrix m_mWorld = sm::Matrix::Identity;
		sm::Matrix m_mViewInverse = sm::Matrix::Identity;
		sm::Matrix m_mProjection = sm::Matrix::Identity;

		sm::Vector2  m_v2DownPoint = sm::Vector2::Zero;

		int m_width = 0;
		int m_height = 0;

		bool m_bControlDownToZoom = false;

		int m_nMouseWheelDelta = 0;

		sm::Vector2 m_v2OldMousePos = { 0,0 };

		RECT m_rcDrag;                          // Rectangle within which a drag can be initiated.
		DirectX::XMFLOAT3 m_vVelocity;         // Velocity of camera
		DirectX::XMFLOAT3 m_vVelocityDrag;      // Velocity drag force
		float m_fDragTimer;                     // Countdown timer to apply drag
		float m_fTotalDragTimeToZero;           // Time it takes for velocity to go from full to 0
		DirectX::XMFLOAT2 m_vRotVelocity = { 0,0 };       // Velocity of camera

		bool MOUSE_L_BUTTON_DOWN = false;
		bool MOUSE_R_BUTTON_DOWN = false;
		bool MOUSE_M_BUTTON_DOWN = false;
		bool KB_ALT_BUTTON_DOWN = false;

		bool LEFT_BUTTON_DOWN = false;
		bool RIGHT_BUTTON_DOWN = false;
		bool SHIFT_BUTTON_DOWN = false;
	};

}
#include "..\Managers\DxDeviceManager.h"
#include "DxCameraOrbital.h"

#include <cmath>
#include <Vector>
//#include "TextOutDebug.h"
//#include "..\SystemLib\libtools.h"
////#include <boost/math/special_functions/relative_difference.hpp>
//#include "..\SystemLib\pugixml.hpp"
//#include "..\DirectXTK\Inc\SimpleMath.h"
////#include <DirectXMath.h>
//

#include "Utils\MathUtils.h"
#include "Utils\StrUtils.h"

using namespace rldx;
using namespace utils;


void DxCameraOrbital::SetFieldOfView(float value)
{
	m_geometryData.fFieldOfView = value;
}

float rldx::DxCameraOrbital::GetFieldOfView() const
{
	return m_geometryData.fFieldOfView;
}

float rldx::DxCameraOrbital::GetAspectRatio() const
{
	return m_geometryData.fAspect;
}

void DxCameraOrbital::SetWindow(int width, int height)
{
	m_width = width;
	m_height = height;
}

void DxCameraOrbital::SetLookAt(const sm::Vector3& pos)
{
	m_geometryData.v3LookAt = pos;
}

void rldx::DxCameraOrbital::SetRotate(float yaw, float pitch)
{
	m_geometryData.fYaw = yaw;
	m_geometryData.fPitch = pitch;
}


const sm::Matrix& DxCameraOrbital::GetViewMatrix()
{
	CalculateEyePosition_Trigonometric();
	DirectX::XMStoreFloat4x4(&m_mView, DirectX::XMMatrixLookAtLH(m_geometryData.v3EyePosition, m_geometryData.v3LookAt, sm::Vector3::Up));

	return m_mView;
}

/// <summary>
/// Calculate eye position using "pythoras"
/// </summary>
void DxCameraOrbital::CalculateEyePosition_Trigonometric()
{	/*
		Based on math:

		x = rsin θ cos φ
		y = rsin θ sin φ
		z = rcos θ
	*/

	auto y = -m_geometryData.fYaw;
	auto p = -m_geometryData.fPitch;
	auto r = m_geometryData.fRadius;

	auto& eye = m_geometryData.v3EyePosition; // for zoom/pan

	// the reason why this is NOT just the simple expression above, is that is gives a very wonky eye movement
	// TODO: this works:
	eye.x = r * cos(p) * cos(y);
	eye.y = r * sin(p);
	eye.z = r * cos(p) * sin(y);

	eye += m_geometryData.v3LookAt; // add zoom/pan
}
/// <summary>
///  Set eye pos with cartesian coords
/// </summary>
void DxCameraOrbital::SetEyePosition(const sm::Vector3& position)
{
	/*---------------------------------------------
	cartesian (x,y,z) to spherical (r, theta, phi):

	r = sqrt(x^2 + y^2 + z^2)
	theta = atan(y/x)
	phi = acos(z/r)

	z / radius = cos(p) *sin(y)
	------------------------------------------------*/
	auto eye = (position)-m_geometryData.v3LookAt;

	m_geometryData.fRadius = (float)sqrt(eye.x * eye.x + eye.y * eye.y + eye.z * eye.z);
	m_geometryData.fPitch = -asin(eye.y / m_geometryData.fRadius);
	m_geometryData.fYaw = -atan2(eye.z, eye.x);

	// update stuff
	CalculateEyePosition_Trigonometric();
}


void DxCameraOrbital::CalculateEyePostion_RotationMatrix()
{
	m_geometryData.v3EyePosition = sm::Vector3::Transform(sm::Vector3::Backward, sm::Matrix::CreateFromYawPitchRoll(m_geometryData.fYaw, m_geometryData.fPitch, 0));
	m_geometryData.v3EyePosition *= m_geometryData.fRadius;
	m_geometryData.v3EyePosition += m_geometryData.v3LookAt;
}



const sm::Matrix& DxCameraOrbital::GetWorldMatrix()
{
	//float distance;      // Straight line distance between the camera and look at point

// Calculate the camera position using the distance and angles

	/*m_v3EyePosition = {
	m_zoom * -sinf(m_yaw) * cosf(m_ ),
	m_zoom * -sinf(m_pitch),
	-m_zoom * cosf(m_yaw) * cosf(m_pitch)
	};*/

	/*auto mX = sm::Matrix::CreateRotationX(m_yaw);
	auto mY = sm::Matrix::CreateRotationX(m_pitch);

	m_v3EyePosition = sm::Vector3::Transform(sm::Vector3::*/

	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	//Calculate the relative position of the camera
	//m_v3EyePosition = sm::Vector3::Transform(sm::Vector3::Backward, sm::Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, 0));
	//////Convert the relative position to the absolute position
	//m_v3EyePosition *= m_fRadius;
	//m_v3EyePosition += m_v3LookAt;

	//m_v3EyePosition += {1, 1, 1};
	//Calculate a new viewmatrix
	//m_mView = sm::Matrix::CreateLookAt(m_v3EyePosition, m_v3LookAt, sm::Vector3::Up);

	using namespace DirectX;
	XMStoreFloat4x4(&m_mWorld, sm::Matrix::CreateFromYawPitchRoll(m_geometryData.m_world_yaw, m_geometryData.m_world_pitch, 0));

	return m_mWorld;
}
;

sm::Vector3 DxCameraOrbital::GetEyePt() const
{
	return m_geometryData.v3EyePosition;
}

sm::Vector3 rldx::DxCameraOrbital::GetLookAt() const
{
	return m_geometryData.v3LookAt;
}

DirectX::XMVECTOR DxCameraOrbital::ScreenToVector(float screenx, float screeny) const
{
	float x = -(screenx - m_width / 2.f) / (m_geometryData.fRadius * m_width / 2.f);
	float y = (screeny - m_height / 2.f) / (m_geometryData.fRadius * m_height / 2.f);

	float z = 0.0f;
	float mag = x * x + y * y;

	if (mag > 1.0f)
	{
		float scale = 1.0f / sqrtf(mag);
		x *= scale;
		y *= scale;
	}
	else
		z = sqrtf(1.0f - mag);

	return DirectX::XMVectorSet(x, y, z, 0);
}

float rldx::DxCameraOrbital::GetRadius() const
{
	return m_geometryData.fRadius;
}

void DxCameraOrbital::OnBegin(int x, int y)
{
	m_bDrag = true;
	//m_qdown = quat;
	//m_v2DownPoint = ScreenTosm::Vector(float(x), float(y));
}

void DxCameraOrbital::OnMove(int x, int y)
{
	using namespace DirectX;
	if (m_bDrag)
	{
		sm::Vector2 v2CurrentPoint = ScreenToVector(float(x), float(y));

		//m_vMouseDelta = (v2CurrentPoint - m_v2DownPoint);

		//m_v2DownPoint = v2CurrentPoint;
#ifdef DEBUG
		//TextOutDebug::drawString(
		//	"Mouse Pose: ( " + std::to_string(x) + ", " + std::to_string(y) + " )\n\r"
		//	"sm::Vector: ( " + std::to_string(v2CurrentPoint.x) + ", " + std::to_string(v2CurrentPoint.y) + " )\n\r"
		//	"Delta ( " + std::to_string(m_vMouseDelta.x) + ", " + std::to_string(m_vMouseDelta.y) + " )\n\r"
		//	"Yaw/Pitch( " + std::to_string(data.m_yaw) + ", " + std::to_string(data.m_pitch) + " )\n\r"
		//	"Cam Pos: " + std::to_string(data.m_v3EyePosition) +
		//	//"Cam Pos: ( " + std::to_string(m_v3EyePosition.x) + ", " + std::to_string(m_v3EyePosition.x) + " )\n\r"
		//	"Cam LookAt: ( " + std::to_string(data.m_v3LookAt.x) + ", " + std::to_string(data.m_v3LookAt.y) + " )\n\r"

		//);
#endif // DEBUG													   

		//rotateCameraRight(m_vMouseDelta.x);

		//m_v3EyePosition = sm::Vector3::Transform(m_v3EyePosition, sm::Matrix::CreateTranslation(m_v3LookAt));

		//m_v3EyePosition = sm::Vector3::Transform(m_v3EyePosition, sm::Matrix::CreateFromYawPitchRoll(m_vMouseDelta.x, -m_vMouseDelta.y, 0));

		//m_v3EyePosition = sm::Vector3::Transform(m_v3EyePosition, sm::Matrix::CreateTranslation(-m_v3LookAt));
	}
	//else
		//m_vMouseDelta = { 0,0 };
}

void DxCameraOrbital::OnEnd()
{
	//m_vMouseDelta = { 0,0 };
	m_bDrag = false;

	//m_v3EyePosition = v3NewPosition;
}

void rldx::DxCameraOrbital::RotateCamera()
{
	rotateCamera_Yaw(m_vRotVelocity.x);
	rotateCamera_Pitch(m_vRotVelocity.y);

	//rldx::DxDeviceManager::GetInstance().GetDebugTextWriter()->AddString(L"Rotate: (" + to_wstring(libtools::ToDegrees(m_geometryData.fPitch)) + L", " + to_wstring(libtools::ToDegrees(m_geometryData.fYaw)) + L")");

	auto radToDegFactor = (180.0f / DirectX::XM_PI);
	rldx::DxDeviceManager::GetInstance().GetDebugTextWriter()->AddString(L"Rotate: " + F2ToWString({ m_geometryData.fPitch * radToDegFactor , m_geometryData.fYaw * radToDegFactor }));
}

void DxCameraOrbital::rotateCamera_Yaw(float angle)
{
	if (SHIFT_BUTTON_DOWN)
	{
		m_geometryData.m_world_yaw += angle;
	}
	else
	{
		m_geometryData.fYaw += angle;
	}



	const auto fullCircle = 2.0f * DirectX::XM_PI;
	m_geometryData.fYaw = fmodf(m_geometryData.fYaw, fullCircle);
	if (m_geometryData.fYaw < 0.0f) {
		m_geometryData.fYaw = fullCircle + m_geometryData.fYaw;
	}

}

void DxCameraOrbital::rotateCamera_Pitch(float angle)
{
	if (SHIFT_BUTTON_DOWN)
	{
		m_geometryData.m_world_pitch -= angle;
	}
	else
	{
		m_geometryData.fPitch -= angle;
	}




	const auto fullCircle = 2.0f * DirectX::XM_PI;
	m_geometryData.fPitch = fmodf(m_geometryData.fPitch, fullCircle);
	if (m_geometryData.fPitch < 0.0f) {
		m_geometryData.fPitch = fullCircle + m_geometryData.fPitch;
	}

	return;


	// TODO: make these values into const(s) vars
	if (m_geometryData.fPitch > DirectX::XM_PIDIV2 - 0.01f)
	{
		m_geometryData.fPitch = DirectX::XM_PIDIV2 - 0.01f;
	}

	if (m_geometryData.fPitch < -(DirectX::XM_PIDIV2 - 0.01f))
	{
		m_geometryData.fPitch = -(DirectX::XM_PIDIV2 - 0.01f);
	}

	//if (data.m_world_pitch > DirectX::XM_PIDIV2 - 0.01)
		//{
		//	data.m_world_pitch = DirectX::XM_PIDIV2 - 0.01;
		//}

		//if (data.m_world_pitch < -(DirectX::XM_PIDIV2 - 0.01))
		//{
		//	data.m_world_pitch = -(DirectX::XM_PIDIV2 - 0.01);
		//}

}

//void CustomOrbitalCamera::rotateCameraRight(float angle)
//{
//	if (SHIFT_BUTTON_DOWN)
//	{
//		data.m_world_yaw -= angle;
//	}
//	else
//	{
//		data.m_yaw -= angle;
//	}
//}

void DxCameraOrbital::OnWheel(float delta)
{
	/*sm::Vector3 dir = m_v3LookAt - m_v3EyePosition;

	m_v3EyePosition += dir * interpolationFactor / 100.f;

	TextOutDebug::drawString("( " + std::to_string(interpolationFactor) + ", " +
		std::to_string(m_v3EyePosition.x) + " ," +
		std::to_string(m_v3EyePosition.y) + " ," +
		std::to_string(m_v3EyePosition.z) + " ,"

	);*/
}

sm::Matrix& DxCameraOrbital::GetProjMatrix()
{
	//m_projDirty = false;

	float aspectRatio = float(m_width) / float(m_height);

	//if (m_bLHCcoords) // Lefthanded coord sys?
	//{
	//	DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)&m_mProjection,
	//		DirectX::XMMatrixPerspectiveFovLH(fFieldOfView, aspectRatio, nearDistance, farDistance)
	//	);

	//	return m_mProjection;
	//}
	//else
	//{
		/*DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)&m_mProjection,
			DirectX::XMMatrixPerspectiveFovLH(fFieldOfView, aspectRatio, nearDistance, farDistance)
		);*/

	using namespace DirectX;
	XMStoreFloat4x4(&m_mProjection, DirectX::XMMatrixPerspectiveFovLH(m_geometryData.fFieldOfView, aspectRatio, m_geometryData.nearDistance, m_geometryData.farDistance));

	return m_mProjection;
	//}
}

void DxCameraOrbital::SetProjParams(_In_ float fFOV, _In_ float fAspect, _In_ float fNearPlane = 0.01f, _In_ float fFarPlane = 100.0f)
{
	// Set attributes for the projection matrix
	m_geometryData.fFieldOfView = fFOV;
	//m_fAsp = fAspect;
	m_geometryData.nearDistance = fNearPlane;
	m_geometryData.farDistance = fFarPlane;
	m_geometryData.fAspect = fAspect;

	DirectX::XMMATRIX mProj = DirectX::XMMatrixPerspectiveFovLH(m_geometryData.fFieldOfView, fAspect, m_geometryData.nearDistance, m_geometryData.farDistance);

	//m_mProjection = mProj;
	XMStoreFloat4x4(&m_mProjection, mProj);
}



//--------------------------------------------------------------------------------------
// Figure out the velocity based on keyboard input & drag if any
//--------------------------------------------------------------------------------------
void DxCameraOrbital::UpdateVelocity(_In_ float fElapsedTime)
{
#if 0
	using namespace DirectX;
	XMVector vGamePadRightThumb = XMVectorSet(m_vGamePadRightThumb.x, -m_vGamePadRightThumb.z, 0, 0);

	XMVector vMouseDelta = XMLoadFloat2(&m_vMouseDelta);
	XMVector vRotVelocity = vMouseDelta * m_fRotationScaler + vGamePadRightThumb * 0.02f;

	XMStoreFloat2(&m_vRotVelocity, vRotVelocity);

	XMVector vKeyboardDirection = XMLoadFloat3(&m_vKeyboardDirection);
	XMVector vGamePadLeftThumb = XMLoadFloat3(&m_vGamePadLeftThumb);
	XMVector vAccel = vKeyboardDirection + vGamePadLeftThumb;

	// Normalize sm::Vector so if moving 2 dirs (left & forward),
	// the camera doesn't move faster than if moving in 1 dir
	vAccel = XMVector3Normalize(vAccel);

	// Scale the acceleration sm::Vector
	vAccel *= m_fMoveScaler;

	if (m_bMovementDrag)
	{
		// Is there any acceleration this frame?
		if (XMVectorGetX(XMVector3LengthSq(vAccel)) > 0)
		{
			// If so, then this means the user has pressed a movement key
			// so change the velocity immediately to acceleration
			// upon keyboard input.  This isn't normal physics
			// but it will give a quick response to keyboard input
			XMStoreFloat3(&m_vVelocity, vAccel);

			m_fDragTimer = m_fTotalDragTimeToZero;

			XMStoreFloat3(&m_vVelocityDrag, vAccel / m_fDragTimer);
		}
		else
		{
			// If no key being pressed, then slowly decrease velocity to 0
			if (m_fDragTimer > 0)
			{
				// Drag until timer is <= 0
				XMVector vVelocity = XMLoadFloat3(&m_vVelocity);
				XMVector vVelocityDrag = XMLoadFloat3(&m_vVelocityDrag);

				vVelocity -= vVelocityDrag * fElapsedTime;

				XMStoreFloat3(&m_vVelocity, vVelocity);

				m_fDragTimer -= fElapsedTime;
			}
			else
			{
				// Zero velocity
				m_vVelocity = XMFLOAT3(0, 0, 0);
			}
		}
	}
	else
	{
		// No drag, so immediately change the velocity
		XMStoreFloat3(&m_vVelocity, vAccel);
	}
#endif
}

void rldx::DxCameraOrbital::MoveFrame(float fElapsedTime)
{
	if (IsMouseMButtonDown())
	{
		UpdateMouseDelta();
	}

	if (MOUSE_L_BUTTON_DOWN && m_bDrag)
	{
		RotateCamera();
	}
	else if (MOUSE_M_BUTTON_DOWN && m_bDrag)
	{
		MoveLookAt();
	}
	else if (m_nMouseWheelDelta != 0)
	{
		Zoom();
	}
}


void rldx::DxCameraOrbital::MoveLookAt()
{
	MoveCameraRight(static_cast<float>(m_vMouseDelta.x) * m_fCameraLookAtMoveSpeed);// *0.1f * m_zoom * 1.0f);
	MoveCameraUp(static_cast<float>(m_vMouseDelta.y) * m_fCameraLookAtMoveSpeed);// *0.1f * m_zoom * 1.0f);		
	// TODO: reablke
	rldx::DxDeviceManager::DebugTextWriter()->AddString(L"Look-At: (" + std::to_wstring(m_geometryData.v3LookAt.x) + L", " + std::to_wstring(m_geometryData.v3LookAt.x) + L" )");
}

void rldx::DxCameraOrbital::Zoom()
{
	m_geometryData.fRadius -= static_cast<float>(m_nMouseWheelDelta) * m_geometryData.fRadius * m_fZoomScaler;
	m_nMouseWheelDelta = 0;

	rldx::DxDeviceManager::DebugTextWriter()->AddString(L"Zoom Distance: (" + std::to_wstring(m_geometryData.fRadius) + L" )");
}

void DxCameraOrbital::UpdateMouseDelta()
{
	if (m_bDrag)
	{
		using namespace DirectX;
		// Get current position of mouse
		POINT ptCurMousePos;
		GetCursorPos(&ptCurMousePos);

		// Calc how _far it's moved since last frame
		POINT ptCurMouseDelta = { 0,0 };
		ptCurMouseDelta.x = ptCurMousePos.x - m_ptLastMousePosition.x;
		ptCurMouseDelta.y = ptCurMousePos.y - m_ptLastMousePosition.y;

		// Record current position for next time
		m_ptLastMousePosition = ptCurMousePos;

		/*
			if (m_bResetCursorAfterMove && DXUTIsActive())
			{
				// Set position of camera to center of desktop,
				// so it always has room to move.  This is very useful
				// if the cursor is hidden.  If this isn't done and cursor is hidden,
				// then invisible cursor will hit the edge of the screen
				// and the user can't tell what happened
				POINT ptCenter;

				// Get the center of the current monitor
				MONITORINFO mi;
				mi.cbSize = sizeof(MONITORINFO);
				DXUTGetMonitorInfo(DXUTMonitorFromWindow(DXUTGetHWND(), MONITOR_DEFAULTTONEAREST), &mi);
				ptCenter.x = (mi.rcMonitor.left + mi.rcMonitor.right) / 2;
				ptCenter.y = (mi.rcMonitor.top + mi.rcMonitor.bottom) / 2;
				SetCursorPos(ptCenter.x, ptCenter.y);
				m_ptLastMousePosition = ptCenter;
			}
			*/
			// Smooth the relative mouse data over a few frames so it isn't
			// jerky when moving slowly at low frame rates.
		float fPercentOfNew = 1.0f / m_fFramesToSmoothMouseData;
		float fPercentOfOld = 1.0f - fPercentOfNew;
		m_vMouseDelta.x = m_vMouseDelta.x * fPercentOfOld + ptCurMouseDelta.x * fPercentOfNew;
		m_vMouseDelta.y = m_vMouseDelta.y * fPercentOfOld + ptCurMouseDelta.y * fPercentOfNew;

		m_vRotVelocity.x = m_vMouseDelta.x * m_fRotationScaler;
		m_vRotVelocity.y = m_vMouseDelta.y * m_fRotationScaler;

		//if (m_nMouseWheelDelta)
		//{
		//	m_fRadius -= static_cast<float>(m_nMouseWheelDelta) * 0.1f;//m_fRadius * 0.1f / 220.0f;
		//}
		///*m_fRadius = std::min<float>(fMaxRadius, m_fRadius);
		//m_fRadius = std::max<float>(fMinRadius, m_fRadius);*/
		//m_nMouseWheelDelta = 0;

		//// Get the inverse of the arcball's rotation matrix
		//XMMATRIX mCameraRot = XMMatrixInverse(nullptr, sm::Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, 0));

		//// Transform sm::Vectors based on camera's rotation matrix
		//XMVector vWorldUp = XMVector3TransformCoord(g_XMIdentityR1, mCameraRot);
		//XMVector vWorldAhead = XMVector3TransformCoord(g_XMIdentityR2, mCameraRot);

		//XMVector xmEye = XMLoadFloat3(&m_v3EyePosition);
		//XMVector xmMouseWorld = XMLoadFloat4(&m_v3Mouse3dWorld);

		//XMVector v3ZoomDirection = xmEye - xmMouseWorld;

		////DirectX::SimpleMathVector4(v3ZoomDirection);
		//// Transform the position interpolationFactor by the camera's rotation
		//XMVector vPosDeltaWorld = XMVector3TransformCoord(vPosDelta, mCameraRot);

		//// Move the lookAt position
		//XMVector vLookAt = XMLoadFloat3(&m_vLookAt);
	}
};
//void NewCamera::MoveFrame(float time)
//{
//	//m_vMouseDelta *= time;
//}


static void writeValueToFile(std::ofstream& f, void* pData, int _byte_count)
{
	// rENABLEs
	//f.write(static_cast <char*> (pData), _byte_count);
}

void DxCameraOrbital::save_to_disk(const std::wstring& _path)
{
	//ofstream ofsOut(_path, ios::binary);










}

void DxCameraOrbital::MoveCameraForward(float amount)
{
	sm::Vector3 forward = m_geometryData.v3LookAt - m_geometryData.v3EyePosition;

	forward.Normalize();
	forward = forward.Cross(sm::Vector3::Up); //calculate the real FORWARD??? (is this right?)
	forward.y = 0;
	forward.Normalize();

	m_geometryData.v3LookAt += forward * amount;
};

void rldx::DxCameraOrbital::MoveCameraRight(float amount)
{
	sm::Vector3 right = (m_geometryData.v3LookAt - m_geometryData.v3EyePosition); //calculate forward
	right.Normalize();
	right = right.Cross(sm::Vector3::Up); //calculate the real right
	right.y = 0;
	right.Normalize();
	m_geometryData.v3LookAt += (right * amount);
}

void rldx::DxCameraOrbital::RayCast(const DirectX::SimpleMath::Vector2& ptCursor, const DirectX::SimpleMath::Vector2& screenDims,
									DirectX::SimpleMath::Vector3& _ray,
									DirectX::SimpleMath::Vector3& _origin
)
{
	//Normalized device coordinates
	sm::Vector3 p;
	p.x = (2.0f * p.x) / screenDims.x - 1.0f;
	p.y = 1.0f - (2.0f * p.y) / screenDims.y;

	DirectX::XMVECTOR det; //Determinant, needed for matrix inverse function call
	sm::Vector3 origin = sm::Vector3(p.x, p.y, 0);
	sm::Vector3 faraway = sm::Vector3(p.x, p.y, 1);

	DirectX::XMMATRIX invViewProj = DirectX::XMMatrixInverse(&det, GetViewMatrix() * GetProjMatrix());
	sm::Vector3 rayorigin = DirectX::XMVector3Transform(origin, invViewProj);
	sm::Vector3 rayend = DirectX::XMVector3Transform(faraway, invViewProj);
	sm::Vector3 raydirection = rayend - rayorigin;
	raydirection.Normalize();




	auto mouseNear = DirectX::XMVectorSet((float)ptCursor.x, (float)ptCursor.y, 0.0f, 0.0f);
	auto mouseFar = DirectX::XMVectorSet((float)ptCursor.x, (float)ptCursor.y, 1.0f, 0.0f);
	auto unprojectedNear = DirectX::XMVector3Unproject(mouseNear, 0, 0, screenDims.x, screenDims.y, 0.0f, 1.0f,
													   GetProjMatrix(), GetViewMatrix(), DirectX::XMMatrixIdentity());

	auto unprojectedFar = DirectX::XMVector3Unproject(mouseFar, 0, 0, screenDims.x, screenDims.y, 0.0f, 1.0f,
													  GetProjMatrix(), GetViewMatrix(), DirectX::XMMatrixIdentity());

	DirectX::XMVECTOR hrResult = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(unprojectedFar, unprojectedNear));
	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, hrResult);

	_ray = direction;
	_origin = unprojectedFar;
}

HRESULT DxCameraOrbital::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//   CBaseCamera::HandleMessages(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{



		case WM_RBUTTONDOWN:

		case WM_MBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_LBUTTONDBLCLK:
		{

			// Capture the mouse, so if the mouse button is
			// released outside the window, we'll get the WM_LBUTTONUP message
			SetCapture(hWnd);
			GetCursorPos(&m_ptLastMousePosition);
			//return TRUE;
		}
		break;


		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_LBUTTONUP:
		{
			//// Update member var state
			//if (uMsg == WM_LBUTTONUP)
			//{
			//	m_bMouse	ButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_LEFT_BUTTON;
			//}
			//if (uMsg == WM_MBUTTONUP)
			//{
			//	m_bMouseMButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_MIDDLE_BUTTON;
			//}
			//if (uMsg == WM_RBUTTONUP)
			//{
			//	m_bMouseRButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_RIGHT_BUTTON;
			//}

			// Release the capture if no mouse buttons down
			if (!MOUSE_R_BUTTON_DOWN &&
				!MOUSE_L_BUTTON_DOWN &&
				!MOUSE_M_BUTTON_DOWN)
			{
				ReleaseCapture();
				OnEnd();

			}
		}

		case WM_CAPTURECHANGED:
		case WM_KILLFOCUS:
		case WM_MOUSELEAVE:
		{
			/*	MOUSE_R_BUTTON_DOWN = false;
				MOUSE_L_BUTTON_DOWN = false;
				MOUSE_M_BUTTON_DOWN = false;
				SHIFT_BUTTON_DOWN = false;
				m_bDrag = false;
				auto DEBUG_1 = 1;*/


				// Release the capture if no mouse buttons down
			if (!MOUSE_R_BUTTON_DOWN &&
				!MOUSE_L_BUTTON_DOWN &&
				!MOUSE_M_BUTTON_DOWN)
			{
				ReleaseCapture();
				OnEnd();

			}
		}

		break;
	}


	if (uMsg == WM_KEYUP)
	{
		switch (wParam)
		{
			case VK_SHIFT: SHIFT_BUTTON_DOWN = false;
				break;
		}
	};


	//} re-enable?
	/*if (uMsg == WM_SYSKEYDOWN && uMsg == VK_MENU) {
		m_transformExternal = nullptr;
	}*/

	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
			/*case VK_LMENU:
			case VK_RMENU:
			{
				KB_ALT_BUTTON_DOWN = true;
			}
			break;*/


			case VK_HOME:
				//m_v3LookAt = { 0, 0, 0 };
				SetEyePosition(m_geometryData.v3EyePosition);
				break;

			case 'W':
			case VK_PRIOR:
				MoveCameraForward(m_fCameraMoveSpeed);
				break;

			case 'S':
			case VK_NEXT:
				MoveCameraForward(-m_fCameraMoveSpeed);
				break;

			case VK_SHIFT: SHIFT_BUTTON_DOWN = true;
				break;

				//rotate with ARROW KEYS
			case VK_LEFT:
				rotateCamera_Yaw(-m_fCameraRotate);
				break;
			case VK_RIGHT:
				rotateCamera_Yaw(m_fCameraRotate);
				break;

			case VK_UP:
				rotateCamera_Pitch(-m_fCameraRotate);
				break;
			case VK_DOWN:
				rotateCamera_Pitch(m_fCameraRotate);
				break;
		}
	};

	if (uMsg == WM_MOUSEWHEEL)
	{
		// Update member var state
		auto zDelta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);
		//m_nMouseWheelDelta += (short)HIWORD(wParam);
		m_nMouseWheelDelta = (GET_WHEEL_DELTA_WPARAM(wParam));

		//m_fRadius -= ((float)m_nMouseWheelDelta) / 10.f;



	}
	if (uMsg == WM_MOUSEMOVE)
	{
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		OnMove(xPos, yPos);
	}

	if (uMsg == WM_RBUTTONDOWN)
	{
		MOUSE_R_BUTTON_DOWN = true;
		m_bDrag = true;

		//SetCapture(hWnd);
		//int xPos = LOWORD(lParam);
		//int yPos = HIWORD(lParam);

		//OnBegin(xPos, yPos);
	}
	if (uMsg == WM_RBUTTONUP)
	{
		MOUSE_R_BUTTON_DOWN = false;
		m_bDrag = false;
		//ReleaseCapture();
		//OnEnd();
	}

	if (uMsg == WM_LBUTTONDOWN)
	{
		MOUSE_L_BUTTON_DOWN = true;
		m_bDrag = true;

		/*SetCapture(hWnd);
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		OnBegin(xPos, yPos);*/
	}
	if (uMsg == WM_LBUTTONUP)
	{
		MOUSE_L_BUTTON_DOWN = false;
		m_bDrag = false;
		/*ReleaseCapture();
		OnEnd();*/
	}

	if (uMsg == WM_MBUTTONDOWN)
	{
		MOUSE_M_BUTTON_DOWN = true;
		m_bDrag = true;
		/*SetCapture(hWnd);
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		OnBegin(xPos, yPos);*/
	}

	if (uMsg == WM_MBUTTONUP)
	{
		MOUSE_M_BUTTON_DOWN = false;
		m_bDrag = false;

		/*	ReleaseCapture();
			OnEnd();
		*/
	}

	if (uMsg == WM_KILLFOCUS)
	{
		MOUSE_M_BUTTON_DOWN = false;
		ReleaseCapture();
		OnEnd();

		return TRUE;
	}

	if (uMsg == WM_SETFOCUS)
	{
		return TRUE;
	}


	return TRUE;
}




//if (((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) && m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
//	((uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) && m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
//	((uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) && m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON))
//{
//	int iMouseX = (short)LOWORD(lParam);
//	int iMouseY = (short)HIWORD(lParam);
//	OnBegin(iMouseX, iMouseY);
//}

//if (((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) && m_nTranslateButtonMask & MOUSE_LEFT_BUTTON) ||
//	((uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) && m_nTranslateButtonMask & MOUSE_MIDDLE_BUTTON) ||
//	((uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) && m_nTranslateButtonMask & MOUSE_RIGHT_BUTTON))
//{
//	int iMouseX = (short)LOWORD(lParam);
//	int iMouseY = (short)HIWORD(lParam);
//	OnBegin(iMouseX, iMouseY);
//}

//if (((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) && m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
//	((uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) && m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
//	((uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) && m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON))
//{
//	int iMouseX = (short)LOWORD(lParam);
//	int iMouseY = (short)HIWORD(lParam);
//	OnBegin(iMouseX, iMouseY);
//}

//if (uMsg == WM_MOUSEMOVE)
//{
//	int iMouseX = (short)LOWORD(lParam);
//	int iMouseY = (short)HIWORD(lParam);
//	OnMove(iMouseX, iMouseY);
//	//m_ViewArcBall.OnMove(iMouseX, iMouseY);
//	//m_PanArcBall.OnMove(iMouseX, iMouseY);
//}

//if ((uMsg == WM_LBUTTONUP && m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
//	(uMsg == WM_MBUTTONUP && m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
//	(uMsg == WM_RBUTTONUP && m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON))
//{
//	OnEnd();
//}

//if ((uMsg == WM_LBUTTONUP && m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
//	(uMsg == WM_MBUTTONUP && m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
//	(uMsg == WM_RBUTTONUP && m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON))
//{
//	OnEnd();
//}

//if ((uMsg == WM_LBUTTONUP && m_nTranslateButtonMask & MOUSE_LEFT_BUTTON) ||
//	(uMsg == WM_MBUTTONUP && m_nTranslateButtonMask & MOUSE_MIDDLE_BUTTON) ||
//	(uMsg == WM_RBUTTONUP && m_nTranslateButtonMask & MOUSE_RIGHT_BUTTON))
//{
//	OnEnd();
//}

//if (uMsg == WM_CAPTURECHANGED)
//{
//	if ((HWND)lParam != hWnd)
//	{
//		if ((m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
//			(m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
//			(m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON))
//		{
//			OnEnd();
//		}

//		if ((m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
//			(m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
//			(m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON))
//		{
//			OnEnd();
//		}

//		if ((m_nTranslateButtonMask & MOUSE_LEFT_BUTTON) ||
//			(m_nTranslateButtonMask & MOUSE_MIDDLE_BUTTON) ||
//			(m_nTranslateButtonMask & MOUSE_RIGHT_BUTTON))
//		{
//			OnEnd();
//		}
//	}

//
//	if (uMsg == WM_LBUTTONDOWN ||
//		uMsg == WM_LBUTTONDBLCLK ||
//		uMsg == WM_MBUTTONDOWN ||
//		uMsg == WM_MBUTTONDBLCLK ||
//		uMsg == WM_RBUTTONDOWN ||
//		uMsg == WM_RBUTTONDBLCLK ||
//		uMsg == WM_LBUTTONUP ||
//		uMsg == WM_MBUTTONUP ||
//		uMsg == WM_RBUTTONUP ||
//		/*uMsg == WM_MOUSEWHEEL ||*/
//		uMsg == WM_MOUSEMOVE)
//	{
//		// m_bDragSinceLastUpdateUpdate = true;
//	}
//}
//	}
//	return FALSE;
//}


//double modulo(stdVector <double> xyz) { return sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2] + 1e-130); }
//void cartesian_to_polar(sm::Vector <double> a, double& r, double& lat, double& lon) { r = modulo(a); lon = atan2(a[1], a[0]); lat = acos(a[2] / r); }
//void polar_to_cartesian(double r, double lat, double lon, sm::Vector <double>& a) {
//	a[2] = r * cos(lat);
//	a[0] = r * sin(lat) * cos(lon);
//	a[1] = r * sin(lat) * sin(lon);
//}
//

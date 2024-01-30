#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <SimpleMath.h>

struct FloatConverter
{
	template <typename CONST_BUF_DATA_TYPE>
	inline static constexpr DirectX::XMFLOAT3 GetSNORMFloat3(const CONST_BUF_DATA_TYPE& vIn)
	{
		return
		{
			(static_cast<float>(vIn.x) / 127.0f) - 1.f,
			(static_cast<float>(vIn.y) / 127.0f) - 1.f,
			(static_cast<float>(vIn.z) / 127.0f) - 1.f
		};
	};

	inline static DirectX::XMFLOAT4 GetSNORMFloat4(DirectX::PackedVector::XMUBYTE4 vIn)
	{
		return
		{
			(static_cast<float>(vIn.x) / 127.0f) - 1.f,
			(static_cast<float>(vIn.y) / 127.0f) - 1.f,
			(static_cast<float>(vIn.z) / 127.0f) - 1.f,
			(static_cast<float>(vIn.w) / 127.0f) - 1.f
		};
	};

	template <typename CONST_BUF_DATA_TYPE>
	inline static DirectX::XMFLOAT4 GetFloat4FromHalf4(const CONST_BUF_DATA_TYPE& in)
	{
		return {
			DirectX::PackedVector::XMConvertHalfToFloat(in.x),
			DirectX::PackedVector::XMConvertHalfToFloat(in.y),
			DirectX::PackedVector::XMConvertHalfToFloat(in.z),
			DirectX::PackedVector::XMConvertHalfToFloat(in.w)
		};
	}

	
	template <typename CONST_BUF_DATA_TYPE>
	inline static DirectX::XMFLOAT2 GetFloat2FromHalf2(const CONST_BUF_DATA_TYPE& in)
	{
		return {
			DirectX::PackedVector::XMConvertHalfToFloat(in.x),
			DirectX::PackedVector::XMConvertHalfToFloat(in.y),
		};
	}

	inline static float GetSNORMFloatFromByte(uint8_t in)
	{
		return (static_cast<float>(in) / 127.0f) - 1.f;
	};

	static float GetUNORMFloatFromByte(uint8_t b)
	{
		return static_cast<float>(b) / 255.0f;
	};

};
//
////
////#include "..\..\QtRenderingWidget\Tools\tools.h"
////
////
////struct FloatConverter
////{
////	static tools::SystemClock g_oClock;
////
////	static float randFloatUNORM(size_t resolution = 1000000)
////	{
////		float fRandom = static_cast<float>(rand());
////
////		float fDivider = RAND_MAX;
////
////		float fUnorm = fRandom / fDivider;
////
////		return fUnorm;
////	}
////
////
////
////	static float randFloatSfloat(float distance = 10.0)
////	{
////		auto result = distance * randFloatUNORM() * 2.0f - distance;
////		return result;
////	}
////
////
////
////	template <typename CONST_BUF_DATA_TYPE>
////	static CONST_BUF_DATA_TYPE sign(CONST_BUF_DATA_TYPE value)
////	{
////		return abs(value) / value;
////	}
////
////	template <typename CONST_BUF_DATA_TYPE>
////	static inline constexpr CONST_BUF_DATA_TYPE clamp(const CONST_BUF_DATA_TYPE& _value, CONST_BUF_DATA_TYPE min, CONST_BUF_DATA_TYPE max)
////	{
////		return std::max<CONST_BUF_DATA_TYPE>(std::min<CONST_BUF_DATA_TYPE>(_value, max), min);
////	}
////
////
////
////	static float clip(float n, float lower, float upper) {
////		return std::max<float>(lower, std::min<float>(n, upper));
////	}
////
////	static DirectX::SimpleMath::Vector3 getVector3(const DirectX::SimpleMath::Vector4& _v4)
////	{
////		return DirectX::SimpleMath::Vector3(_v4.x, _v4.y, _v4.z);
////	}
////
////
////	static DirectX::SimpleMath::Vector4 getVector4(const DirectX::SimpleMath::Vector3& _v3)
////	{
////		return DirectX::SimpleMath::Vector4(_v3.x, _v3.y, _v3.z, 0.f);
////	}
////
////
////	static DirectX::XMFLOAT4 DecodeQuaternion(const vector<int16_t>& vShort4Raw)
////	{
////		return DecodeQuaternion(vShort4Raw.data());
////	}
////
////	static DirectX::XMFLOAT4 DecodeQuaternion(const int16_t* vShort4Raw)
////	{
////		DirectX::XMFLOAT4 quat;
////		quat.x = FloatConverter::SNORM16_To_Float(vShort4Raw[0]);
////		quat.y = FloatConverter::SNORM16_To_Float(vShort4Raw[1]);
////		quat.z = FloatConverter::SNORM16_To_Float(vShort4Raw[2]);
////		quat.w = FloatConverter::SNORM16_To_Float(vShort4Raw[3]);
////
////		DirectX::SimpleMath::Quaternion norm_quat = quat;
////		norm_quat.Normalize();
////
////		return norm_quat;
////	}
////
////
////	static constexpr DirectX::XMFLOAT4X4 identity4x4()
////	{
////		return
////		{
////			1, 0, 0, 0,
////			0, 1, 0, 0,
////			0, 0, 1, 0,
////			0, 0, 0, 1
////		};
////	};
////
////	static inline constexpr float SNORM16_To_Float(int16_t in)
////	{
////		/*if (in == 32767)
////			return 1.f;
////
////		if (in == -32768)
////			return -1.f;*/
////
////		float c = in;
////		return (c / (32767.0f));
////	}
////
////	static inline constexpr float SNORM8_To_Float(int8_t in)
////	{
////		/*if (in == 32767)
////			return 1.f;
////
////		if (in == -32768)
////			return -1.f;*/
////
////		float c = in;
////		return (c / (127.0f));
////	}
////
////	static constexpr double rad(double deg)
////	{
////		return (deg / 360.0) * 6.283185307;
////	}
////
////	static constexpr float rad(float deg)
////	{
////		return (deg / 360.0f) * DirectX::XM_2PI;
////	}
////
////	static inline DirectX::XMFLOAT3 normalize_vector(const DirectX::XMFLOAT3& _normal)
////	{
////		float len = length_vector(_normal);
////
////		DirectX::XMFLOAT3 result = _normal;
////
////		result.x /= len;
////		result.y /= len;
////		result.z /= len;
////
////		return result;
////	}
////
////	inline static float length_vector(DirectX::XMFLOAT3 _normal)
////	{
////		return
////			sqrt(
////				_normal.x * _normal.x +
////				_normal.y * _normal.y +
////				_normal.z * _normal.z
////
////			);
////	}
////
////	inline static float length_vector(DirectX::XMFLOAT4 _normal)
////	{
////		return
////			sqrt(
////				_normal.x * _normal.x +
////
////				_normal.y * _normal.y +
////
////				_normal.z * _normal.z +
////
////				_normal.w * _normal.w
////
////			);
////	}
////
////	/*static DirectX::XMFLOAT3 unpack_normal8(uint32_t data)
////	{
////		float x = (((data) & 0xFF) / 127.5f - 1.0f);
////		float y = ((((data) >> 8) & 0xFF) / 127.5f - 1.0f);
////		float z = ((((data) >> 16) & 0xFF) / 127.5f - 1.0f);
////
////		return { x,y,z };
////	}*/
////
////	static sm::Matrix getRotationalPartOfMatrix(const sm::Matrix& _m)
////	{
////		// use only the rotational component for normal/tangent transform
////		sm::Matrix temp = _m;
////		DirectX::SimpleMath::Vector3 Trans;
////		DirectX::SimpleMath::Vector3 Scale;
////		DirectX::SimpleMath::Quaternion Quat;
////		temp.Decompose(Scale, Quat, Trans);
////
////		return DirectX::SimpleMath::Matrix::CreateFromQuaternion(Quat);
////	}
////
////	static DirectX::XMFLOAT4 mul_quat(DirectX::XMFLOAT4 q1, DirectX::XMFLOAT4 q2)
////	{
////		DirectX::XMVECTOR _q1 = DirectX::XMLoadFloat4(&q1);
////		DirectX::XMVECTOR _q2 = DirectX::XMLoadFloat4(&q2);
////
////		DirectX::XMVECTOR _q3 = DirectX::XMQuaternionMultiply(_q1, _q2);
////
////		DirectX::XMFLOAT4 q3;
////		DirectX::XMStoreFloat4(&q3, _q3);
////
////		return q3;
////	}
////
////	static uint8_t SNORM_to_Quant8(float f)
////	{
////		/*if (f > 1.0f)
////			f = 1.0f;
////		if (f < -1.0f)
////			f = -1.0f;		*/
////
////		float c = (f + 1.0f) * 127.0f;
////
////		/*	if (c >= 0)
////				c = c + 0.5f;
////			else c = c - 0.5f;*/
////
////		uint8_t result = static_cast<uint8_t>(c);
////
////		return result;
////
////		//uint8_t b = static_cast<uint8_t>(min<uint8_t>(255, static_cast<uint32_t>(rf)));
////
////		//return b;
////	}
////
////	inline static constexpr float GetUNORMFloatFromByte(uint8_t b)
////	{
////		return static_cast<float>(b) / 255.0f;
////	}
////
////	inline static constexpr float GetUnormFloatFromUint8(uint8_t b)
////	{
////		return static_cast<float>(b) / 255.0f;
////	}
////
////	static constexpr DirectX::XMFLOAT3 GetSNORMFloat3(DirectX::PackedVector::XMBYTE4 vIn)
////	{
////		return
////		{
////			(static_cast<float>(vIn.x) / 127.0f) - 1.f,
////			(static_cast<float>(vIn.y) / 127.0f) - 1.f,
////			(static_cast<float>(vIn.z) / 127.0f) - 1.f
////		};
////	}
////
////	
////	static constexpr DirectX::XMFLOAT4 byte4_to_snorm_float4(DirectX::PackedVector::XMUBYTE4 vIn)
////	{
////		return
////		{
////			(static_cast<float>(vIn.x) / 127.0f) - 1.f,
////			(static_cast<float>(vIn.y) / 127.0f) - 1.f,
////			(static_cast<float>(vIn.z) / 127.0f) - 1.f,
////			(static_cast<float>(vIn.w) / 127.0f) - 1.f
////		};
////	}
////
////	static constexpr DirectX::XMFLOAT3 GetSNORMFloat3(DirectX::PackedVector::XMUBYTE4 vIn)
////	{
////		return
////		{
////			(static_cast<float>(vIn.x) / 127.0f) - 1.f,
////			(static_cast<float>(vIn.y) / 127.0f) - 1.f,
////			(static_cast<float>(vIn.z) / 127.0f) - 1.f,			
////		};
////	}
////
////	static DirectX::XMFLOAT3 GetNormalFromBytes(const DirectX::PackedVector::XMUBYTE4 bytes4)
////	{
////		return
////		{
////			GetSNORMFloatFromByte(bytes4.x),
////			GetSNORMFloatFromByte(bytes4.y),
////			GetSNORMFloatFromByte(bytes4.z)
////		};
////	}
////
////	static constexpr float GetSNORMFloatFromByte(uint8_t b)
////	{
////		return (static_cast<float>(b) / 127.0f) - 1.f;
////	}
////
////	template <typename CONST_BUF_DATA_TYPE, int DIV = (sizeof(CONST_BUF_DATA_TYPE) / 2) - 1>
////	static constexpr float TEST_uint_to_snorm_float(CONST_BUF_DATA_TYPE b)
////	{		
////		auto value = (static_cast<float>(b) / static_cast<float>(DIV)) - 1.f;
////		return value;
////
////		//float i = b;
////
////		////float r1 =  ((i / 255.f) * 2.f) - 1.f;
////		//float r1 = (i / 127.5f) - 1.f;
////
////		//return r1;
////	};
////
////	/*static constexpr float uint_to_snorm_float(uint8_t b)
////	{
////		return ((float)b) / 127.5f - 1.f;
////	};*/
////
////	/*static constexpr uint8_t norm_float_to_byte(float f)
////	{
////		float ret = (f + 1 ) * 127.0f;
////		return ret;
////	};*/
////
////	//thread_local static  DirectX::XMFLOAT3 oFloat3Temp;;
////	static inline DirectX::XMFLOAT3& normalize3(const DirectX::XMFLOAT3& _o)
////	{
////		static DirectX::XMFLOAT3 oFloat3Temp;;
////		// calc length
////		float len = sqrt(_o.x * _o.x + _o.y * _o.y + _o.z * _o.z);
////
////		// dive element by length
////		oFloat3Temp.x = _o.x / len;
////		oFloat3Temp.y = _o.y / len;
////		oFloat3Temp.z = _o.z / len;
////
////		return oFloat3Temp;
////	}
////
////	//thread_local static  DirectX::XMFLOAT4 oFloat3Temp2;;
////	static inline DirectX::XMFLOAT4& normalize4_AsVec3(const DirectX::XMFLOAT4& _o)
////	{
////		DirectX::XMFLOAT4 oFloat3Temp2;
////		// calc length
////		float len = sqrt(_o.x * _o.x + _o.y * _o.y + _o.z * _o.z);
////
////		// dive element by length
////		oFloat3Temp2.x = _o.x / len;
////		oFloat3Temp2.y = _o.y / len;
////		oFloat3Temp2.z = _o.z / len;
////		oFloat3Temp2.w = _o.w;
////
////		return oFloat3Temp2;
////	};
////
////	//static inline uint8_t floatToSNorm8(float _f)
////	//{
////	//	float c = std::clamp(_f, -1.0f, 1.0f);
////
////	//	c = c * (127 - 1);
////	//
////	//	if (c >= 0)
////	//		c = c + 0.5f;
////	//	else c = c - 0.5f;
////
////	//	c = trunc(c);
////
////	//	uint8_t ret = c;
////
////	//	return ret;
////	//
////	//}
////
////	//static constexpr inline float UNorm8ToFloat(uint8_t in)
////	//{
////	//	float ret = in;
////	//	ret = (ret / 127.5f) - 1.f;
////	//
////	//	return ret;
////	//}
////
////	//static inline uint8_t floatToUNorm8(float _f)
////	//{
////	//	float c = std::clamp(_f, 0.0f, 1.0f);
////
////	//	c = c * (127);
////	//
////	//	if (c >= 0)
////	//		c = c + 0.5f;
////	//	else c = c - 0.5f;
////
////	//	c = trunc(c);
////
////	//	uint8_t ret = c;
////
////	//	return ret;
////	//
////	//}
////
////	
////
////	/*static inline DirectX::XMFLOAT4X4 mul_matrix4x4(XMFLOAT4X4& m1, DirectX::XMFLOAT4X4& m2)
////	
////	*/
////	//static constexpr float uint8_to_float1(uint8_t _in)
////	//{
////	//	return  ((((float) _in) / 255.0f) * 2.0f) - 1.0f;
////	//}
////
////	static constexpr bool isSRGB(DXGI_FORMAT format)
////	{
////		switch (format)
////		{
////		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
////		case DXGI_FORMAT_BC1_UNORM_SRGB:
////		case DXGI_FORMAT_BC2_UNORM_SRGB:
////		case DXGI_FORMAT_BC3_UNORM_SRGB:
////		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
////		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
////		case DXGI_FORMAT_BC7_UNORM_SRGB:
////			return true;
////
////		default:
////			return false;
////		}
////
////		return false;
////	}
////
////	//static HRESULT getCompiledPixelShaderFromrResource(
////	//	ID3D11Device* _poDevice,
////	//	ID3D11PixelShader** _poDest,
////
////	//	UINT res_id,
////	//	SIZE_T* size = NULL, LPVOID* _poByteCode = NULL)
////
////	//{
////	//	HRESULT hr = S_OK;
////	//	HRSRC hrsc = FindResource(H_INST, MAKEINTRESOURCE(res_id), RT_RCDATA);
////	//	HGLOBAL hg = LoadResource(H_INST, hrsc);
////	//	void* p = LockResource(hg);
////
////	//	DWORD dwSourceSize = SizeofResource(
////	//		H_INST,
////	//		hrsc
////	//	);
////
////	//	hr = _poDevice->CreatePixelShader(
////	//		p,
////	//		dwSourceSize,
////	//		nullptr,
////
////	//		_poDest
////	//	);
////
////	//	assert(SUCCEEDED(hr));
////
////	//	if (size)
////	//		*size = dwSourceSize;
////
////	//	if (_poByteCode)
////	//		*_poByteCode = p;
////
////	//	return hr;
////	//};
////
////	//static HRESULT getCompiledVertexShaderFromrResource(
////	//	ID3D11Device* _poDevice,
////	//	ID3D11VertexShader** _poDest,
////	//	UINT res_id,
////	//	SIZE_T* size = NULL, LPVOID* _poByteCode = NULL)
////
////	//{
////	//	HRESULT hr = S_OK;
////	//	HRSRC hrsc = FindResource(H_INST, MAKEINTRESOURCE(res_id), RT_RCDATA);
////	//	HGLOBAL hg = LoadResource(H_INST, hrsc);
////	//	void* p = LockResource(hg);
////
////	//	DWORD dwSourceSize = SizeofResource(
////	//		H_INST,
////	//		hrsc
////	//	);
////
////	//	hr = _poDevice->CreateVertexShader(
////	//		p,
////	//		dwSourceSize,
////	//		nullptr,
////	//		_poDest
////	//	);
////
////	//	assert(SUCCEEDED(hr));
////
////	//	if (size)
////	//		*size = dwSourceSize;
////
////	//	if (_poByteCode)
////	//		*_poByteCode = p;
////
////	//	return hr;
////	//};
////
////	//static void box_blur(DirectX::XMFLOAT4* _pRGBA, uint32_t width, uint32_t height)
////	//{
////	//	//XMBYT* pNew = new DirectX::XMFLOAT4 [width * height];
////	//	auto image = [_pRGBA, width, height](int x, int y) -> DirectX::XMFLOAT4
////	//	{
////	//		return _pRGBA[y * width + x];
////	//	};
////
////	//	auto new_image = [_pRGBA, width, height](int x, int y) -> DirectX::XMFLOAT4
////	//	{
////	//		return _pRGBA[y * width + x];
////	//	};
////
////	//	for (size_t y = 0; y < height; y++)
////	//		for (size_t x = 0; x < width; x++)
////	//		{
////	//			// Kernel would not fit!
////	//			if (x < 1 || y < 1 || x + 1 >= width || y + 1 >= height)
////	//				continue;
////	//			// Set P to the average of 9 pixels:
////	//			//		X X X
////	//			//		X P X
////	//			//		X X X
////	//				// Calculate average.
////	//			XMFLOAT3 sum = { 0,0,0 };
////
////	//			sum.x = image(x - 1, y + 1).x; // Top left
////	//			sum.y = image(x - 1, y + 1).y; // Top left
////	//			sum.z = image(x - 1, y + 1).z; // Top left
////
////	//			sum.x += image(x + 0, y + 1).x; // Top center
////	//			sum.y += image(x + 0, y + 1).y; // Top center
////	//			sum.z += image(x + 0, y + 1).z; // Top center
////
////	//			sum.x += image(x + 1, y + 1).x; // Top right
////	//			sum.y += image(x + 1, y + 1).y; // Top right
////	//			sum.z += image(x + 1, y + 1).z; // Top right
////
////	//			sum.x += image(x - 1, y + 0).x; // Mid left
////	//			sum.y += image(x - 1, y + 0).y; // Mid left
////	//			sum.z += image(x - 1, y + 0).z; // Mid left
////
////	//			sum.x += image(x + 0, y + 0).x;  // Current pixel
////	//			sum.y += image(x + 0, y + 0).y;  // Current pixel
////	//			sum.z += image(x + 0, y + 0).z;  // Current pixel
////
////	//			sum.x += image(x + 1, y + 0).x;  // Mid right
////	//			sum.y += image(x + 1, y + 0).y;  // Mid right
////	//			sum.z += image(x + 1, y + 0).z;  // Mid right
////
////	//			sum.x += image(x - 1, y - 1).x;  // Low left
////	//			sum.y += image(x - 1, y - 1).y;  // Low left
////	//			sum.z += image(x - 1, y - 1).z;  // Low left
////
////	//			sum.x += image(x + 0, y - 1).x;  // Low center
////	//			sum.y += image(x + 0, y - 1).y;  // Low center
////	//			sum.z += image(x + 0, y - 1).z;  // Low center
////
////	//			sum.x += image(x + 1, y - 1).x;  /// Low right
////	//			sum.y += image(x + 1, y - 1).y;  /// Low right
////	//			sum.z += image(x + 1, y - 1).z;  /// Low right
////
////	//			//image[x, y] = Sum / 9;
////	//		}
////	//	//Return newImage;
////	//}
////
////	//static void gaussian_blur(uint32_t iHeight, uint32_t iWidth)
////	//{
////	//	// gaussian filter kernel
////	//	float gauss[3][3] = { {1, 2, 1},
////	//						  {2, 4, 2},
////	//	                      {1, 2, 1} };
////
////	//	int convHeight;     // heighLPARAM lParam of convolution filter, gaussian in this case
////	//	int convWidth;      // width value of convolution filter, gaussian in this case
////
////	//	//////gaussian blur/////////
////	//	for (int i = 0; i < iHeight; i++) {
////	//		for (int j = 0; j < iWidth; j++) {
////	//			gaussPixel = 0;
////	//			for (int x = 0; x < convHeight; x++) {
////	//				for (int y = 0; y < convWidth; y++) {
////	//					gaussPixel += OldImage[x - convWidth / 2 + i, y - convHeight / 2 + j] * gauss[i, j];
////	//				}
////	//			}
////	//			NewImage[x, y] = gaussPixel;
////	//		}
////	//	}
////
////	//}
////};
////
//////thread_local DirectX::XMFLOAT3 FloatConverter::oFloat3Temp;
//////thread_local DirectX::XMFLOAT4 FloatConverter::oFloat3Temp2;
////
//////XMFLOAT4 decodeFloat4_From32Bit(uint32_t dw)
//////{
//////	uint8_t* p = reinterpret_cast<uint8_t*>(&dw);
//////
//////	XMFLOAT4 R = { 0,0,0,0 };
//////
//////	R.x = (static_cast<float>(p[0]) / 255.f) - 1.f;
//////	R.y = (static_cast<float>(p[1]) / 255.f) - 1.f;
//////	R.z = (static_cast<float>(p[2]) / 255.f) - 1.f;
//////	R.w = (static_cast<float>(p[3]) / 255.f) - 1.f;
//////
//////	return R;
//////}
//////
//////template <uint32_t BITS_X, uint32_t BITS_Y, uint32_t BITS_Z>
//////XMFLOAT3 ____decode_3_32_to_float(uint32_t dwData)
//////{
//////	uint32_t mask_x = (1 << BITS_X) - 1;
//////	uint32_t mask_y = (1 << BITS_Y) - 1;
//////	uint32_t mask_z = (1 << BITS_Z) - 1;
//////
//////	uint32_t x = dwData & mask_x; // (0b11111111111);
//////
//////	uint32_t y = dwData & (mask_y << BITS_X); // (0b11111111111 << 11);
//////
//////	uint32_t z = dwData & (mask_z << (BITS_X + BITS_Y)); // (0b1111111111 << 21);
//////
//////	y = y >> BITS_X;
//////	z = z >> (BITS_X + BITS_Y);
//////
//////	float _x = ((float)x) / ((float)mask_x);
//////	float _y = ((float)y) / ((float)mask_y);
//////	float _z = ((float)z) / ((float)mask_z);
//////
//////	XMFLOAT3 R;
//////	R.x = _x;
//////	R.y = _y;
//////	R.z = _z;
//////
//////	return R;
//////}
//////
//////XMFLOAT3 _decode_24bit_unorm_to_float3(const vector<int8_t>& Vecdata);
////
//////XMFLOAT4 _decode_48bit_unorm_to_float4(const vector<int8_t>& Vecdata);
////
////
////
//////namespace DX
//////{
//////	// Helper class for COM exceptions
//////	class com_exception : public std::exception
//////	{
//////	public:
//////		com_exception(HRESULT hr) : m_result(hr) {}
//////
//////		const char* what() const noexcept override
//////		{
//////			std::string errMsg(64, '\0');
//////
//////			sprintf_s(&errMsg[0], 64, "Failure with HRESULT of %08X", static_cast<unsigned int>(m_result));
//////
//////			errMsg = errMsg.c_str();  // cut away padding zeros;
//////
//////			_com_error err(m_result);
//////			errMsg += "\n" + NarrowStr(err.ErrorMessage());
//////
//////			return errMsg.c_str();;
//////		}
//////
//////	private:
//////		HRESULT m_result;
//////	};
//////
//////	// Helper utility converts D3D API failures into exceptions.
//////	inline void ThrowIfFailed(HRESULT hr)
//////	{
//////		if (FAILED(hr))
//////		{
//////			throw com_exception(hr);
//////		}
//////	}
//////}

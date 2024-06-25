#pragma once
#include <string>
#include <winerror.h>

std::wstring GetComErrorW(HRESULT hr);

std::string GetComError(HRESULT hr);

bool comAssert_Box(HRESULT hr, std::string _func = "", std::string _operation = "");;

std::string GetComErrorFull(HRESULT hr);

std::string GetComErrorFormated(HRESULT hr, const std::string& message = "");

bool comAssert_LogOnly(HRESULT hr, std::string _func = "", std::string _operation = "");;

// TODO: delete?
//#define COM_COND_THROW(HR) \
//if (!SUCCEEDED(HR)) throw libtools::COMException(HR); \
//
//#define LOADING_FILE(path) (std::string("Loading: ") + std::string(path))
//
//#define COM_ASSERT_BOX(HR, op) libtools::comAssert_Box(HR, __FUNCTION__, op)
//
//#define COM_ASSERT(HR) libtools::comAssert_LogOnly(HR, __FUNCTION__)//\
////assert(SUCCEEDED(HR));\
//
//#define COM_ASSERT_OP(HR, op) libtools::comAssert_LogOnly(HR, __FUNCTION__, op)
////\
//// assert(SUCCEEDED(HR));\
//
//#ifndef ComMessageBox
//#define ComMessageBox(hr, _title, _op) \
//		auto message = wstring(libtools::string_to_wstring(__FUNCTION__) + L"\r\n") \
//	+ wstring(L"Failed: " + wstring(_op) + _wstrPath + L"\r\n") \
//		+ wstring(L"HRESULT: ") + std::to_wstring(hr) + L"\r\n" \
//		+ wstring(L"String: ") + libtools::getComError(hr) + L"\r\n"; \
//	MessageBox(NULL, message.c_str(), _title, MB_OK | MB_ICONERROR);\
//
//#endif

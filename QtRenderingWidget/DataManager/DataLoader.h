#pragma once
#include <vector>
#include <string>


#include <qlist.h>

#include <Utils/ByteStream.h>
#include <Utils/IOUtils.h>

#include <QtRenderingWidget\ExternFunctions\Creators.h>


class DataLoader
{
public:
	DataLoader(CallBackFuncPtr clientCallBack)
		:
		m_assetCallBackFunctionPtr(clientCallBack)
	{}
	
	void LoadData(
		std::vector<std::wstring>* filesToFetch,
		std::vector<std::vector<unsigned char>>* outputBinaryFiles) const;

private:
	void FetchAssetsFromClientCallBack(std::vector<std::wstring>* filesToFetch, std::vector<utils::ByteVector>* outputBinaryFiles) const;
	void FetchAssetUsingLocalCallback(std::vector<utils::ByteVector>* outputBinaryFiles, std::vector<std::wstring>* filesToFetch) const;

private:
	CallBackFuncPtr m_assetCallBackFunctionPtr = nullptr;

};
// TODO: delete if notthing useful
//
//class AssetCallBackManager
//{
//public:
//	CallBackManager(AssetFetchCallbackWrapper assetCallBackFunc)
//		: sm_assetCallBack(assetCallBackFunc) {}
//
//	utils::ByteStream GetFileFromCallBack(const std::wstring& fileName);
//	utils::ByteStream GetFile(const std::wstring& filePath);
//
//private:
//	void CallAssetFetchCallBack(std::vector<std::wstring>& qstrMissingFiles, std::vector<std::vector<unsigned char>>& destBinaries) { GetResourcesFromCallBack(qstrMissingFiles, destBinaries); };
//	static void GetResourcesFromCallBack(const std::vector<std::wstring>& qstrMissingFiles, std::vector<std::vector<unsigned char>>& destBinaries);
//
//private:
//	// the callback function that will be called to fetch files
//	AssetFetchCallbackWrapper sm_assetCallBack;
//};
//
//
//class IRawDataFactory
//{
//public:
//	virtual	utils::ByteStream CreateByteStream(const std::wstring& filePath) = 0;
//};
//
//
//class CallbackRawDataFactory : public IRawDataFactory
//{
//public:
//	CallbackRawDataFactory(AssetFetchCallbackWrapper assetCallBackFunc)
//		: sm_assetCallBack(assetCallBackFunc) {}
//
//	virtual	utils::ByteStream CreateByteStream(const std::wstring& filePath)
//	{
//		return DxResourceManager::GetFileFromCallBack(filePath);
//	}
//
//private:
//	AssetFetchCallbackWrapper sm_assetCallBack = nullptr;
//}
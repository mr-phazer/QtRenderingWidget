#pragma once

// author headers
#include <CommonLibs\Logger\Logger.h>
#include <CommonLibs\Utils\MapUtils.h>
#include <CommonLibs\Utils\ByteStream.h>
#include <CommonLibs\Utils\IOUtils.h>

#include <Rldx/Rldx/Managers/CommonTypes.h>

namespace rldx {

	class CallBackManager
	{




	public:
		CallBackManager(AssetFetchCallbackWrapper assetCallBackFunc)
			: sm_assetCallBack(assetCallBackFunc) {}

		utils::ByteStream GetFileFromCallBack(const std::wstring& fileName);
		utils::ByteStream GetFile(const std::wstring& filePath);

	private:
		void CallAssetFetchCallBack(std::vector<std::wstring>& qstrMissingFiles, std::vector<std::vector<unsigned char>>& destBinaries) { GetResourcesFromCallBack(qstrMissingFiles, destBinaries); };
		static void GetResourcesFromCallBack(const std::vector<std::wstring>& qstrMissingFiles, std::vector<std::vector<unsigned char>>& destBinaries);

	private:
		// the callback function that will be called to fetch files
		AssetFetchCallbackWrapper sm_assetCallBack;
	};


};
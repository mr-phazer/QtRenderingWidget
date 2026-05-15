#include "DataLoader.h"

#include <Rldx/Managers/ResourceManager/DxResourceManager.h>

using namespace std;

void DataLoader::LoadData(std::vector<std::wstring>* filesToFetch, std::vector<std::vector<unsigned char>>* outputBinaryFiles) const
{

	// If Client callback is set, use it for getting the files.
	if (m_assetCallBackFunctionPtr != nullptr)
	{
		FetchAssetsFromClientCallBack(filesToFetch, outputBinaryFiles);
	}
	else {
		FetchAssetUsingLocalCallback(outputBinaryFiles, filesToFetch);
	}
}

void DataLoader::FetchAssetsFromClientCallBack(std::vector<std::wstring>* filesToFetch, std::vector<utils::ByteVector>* outputBinaryFiles) const
{
	QList<QString> filesToFetchQt;
	QList<QByteArray> outBinFilesQt;

	for (const std::wstring& i : *filesToFetch) {
		QString s = QString::fromWCharArray(i.c_str());
		filesToFetchQt.push_back(s);
	}

	m_assetCallBackFunctionPtr(&filesToFetchQt, &outBinFilesQt);

	filesToFetch->clear();

	for (int i = 0; i < filesToFetchQt.count(); i++)
	{
		std::wstring s = filesToFetchQt.at(i).toStdWString();
		filesToFetch->push_back(s);
	}

	for (int i = 0; i < outBinFilesQt.count(); i++)
	{
		auto item = outBinFilesQt.at(i);
		std::vector<unsigned char> s(item.begin(), item.end());
		outputBinaryFiles->push_back(s);
	}
}

void DataLoader::FetchAssetUsingLocalCallback(std::vector<utils::ByteVector>* outputBinaryFiles, std::vector<std::wstring>* filesToFetch) const
{
	// force list to be same size, maybe redundant
	outputBinaryFiles->clear();


	for (const std::wstring& assetPath : *filesToFetch) {

		auto diskFilePath = (utils::IsDiskFile(assetPath)) ? assetPath : rldx::DxResourceManager::GetGameAssetFolder() + assetPath;

		utils::ByteStream assetStream(diskFilePath, false);

		if (!assetStream.IsValid())
		{
			logging::LogWarning(L"Byte stream is not valid. Nothing added to outpo				  ut.");
			continue;
		}

		std::vector<unsigned char> assetBinaryBuffer = std::vector<unsigned char>(assetStream.GetBufferSize());
		assetStream.Read(assetBinaryBuffer.data(), assetBinaryBuffer.size());

		outputBinaryFiles->push_back(assetBinaryBuffer);
	}
}
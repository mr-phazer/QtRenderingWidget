#include "Creators.h"

#include <exception>

#include <CommonLibs\Logger\Logger.h>

#include <Rldx\Rldx\Creators\DxSceneCreator.h>

#include <QtRenderingWidget\QtObjects\Views\QtRenderWidgetView.h>

using namespace logging;
using namespace utils;

static void (*AssetFetchCallBackStored) (QList<QString>* missingFiles, QList<QByteArray>* outBinFiles) = nullptr;

void AssetFetchCallbackWrapper(
	std::vector<std::wstring>* filesToFetch,
	std::vector<std::vector<unsigned char>>* outBinFiles
) {

	// If we don't have callback, use the old debug mode.
	if (AssetFetchCallBackStored == nullptr) {

		// force list to be same size, maybe redundant
		outBinFiles->clear();
		outBinFiles->resize(filesToFetch->size());

		for (const std::wstring& Asset : *filesToFetch) {

			if (IsDiskFile(Asset)) {
				throw std::exception("Cannot accept disk files");
			}

			// TODO: the "GetGameAssetFolder" is only for "local callback" debuggin
			auto filePath = rldx::DxResourceManager::GetGameAssetFolder() + Asset;
			ByteStream newStream(filePath, false);


			if (!newStream.IsValid())
			{
				std::vector<unsigned char> dest = std::vector<unsigned char>();
				outBinFiles->push_back(dest);
				continue;
			}

			// -- resize dest, buffer, and read whole file into it
			std::vector<unsigned char> dest = std::vector<unsigned char>(newStream.GetBufferSize());
			newStream.Read(dest.data(), dest.size());
		}
	}
	
	// If we have callback, use it for getting the files.
	else {

		QList<QString>* filesToFetchQt = new QList<QString>();
		QList<QByteArray>* outBinFilesQt = new QList<QByteArray>();

		for (const std::wstring& i : *filesToFetch) {
			QString s = QString::fromWCharArray(i.c_str());
			filesToFetchQt->push_back(s);
		}

		AssetFetchCallBackStored(filesToFetchQt, outBinFilesQt);
	
		filesToFetch->clear();

		for (int i = 0; i < filesToFetchQt->count(); i++)
		{
			std::wstring s = filesToFetchQt->at(i).toStdWString();
			filesToFetch->push_back(s);
		}

		for (int i = 0; i < outBinFilesQt->count(); i++)
		{
			auto item = outBinFilesQt->at(i);
			std::vector<unsigned char> s(item.begin(), item.end());
			outBinFiles->push_back(s);
		}	
	}
}

QWidget* CreateQRenderingWidget(
	QWidget* parent,
	QString* gameIdString,
	void (*AssetFetchCallBack) (QList<QString>* missingFiles, QList<QByteArray>* outBinFiles),
	void (*AnimPathsBySkeletonCallBack) (QString* skeletonName, QList<QString>* out)
)
{
	AssetFetchCallBackStored = AssetFetchCallBack;

	rldx::DxResourceManager::SetAssetFetchCallback(AssetFetchCallbackWrapper);
	//rldx::DxResourceManager::SetAnimPathsBySkeletonCallBack(AnimPathsBySkeletonCallBack);

	QtRenderWidgetView* poNewRenderingWidget = nullptr;

	try {
		poNewRenderingWidget = new QtRenderWidgetView(parent, *gameIdString); // nullptr = no parent, free floating window			
		poNewRenderingWidget->hide();

		poNewRenderingWidget->GetSceneManager()->GetResourceManager().SetGameIdSting(gameIdString->toStdWString()); // TODO: maybe ONLY store it here?		
	}
	catch (std::exception& e)
	{
#ifdef _DEBUG
		MessageBoxA(reinterpret_cast<HWND>(parent->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
#endif

		logging::LogAction(L"Error: Excpetion: " + utils::ToWString(e.what()));

		delete poNewRenderingWidget;
		return nullptr;
	}

	poNewRenderingWidget->setWindowFlag(Qt::WindowType::Widget);
	poNewRenderingWidget->setWindowTitle("QtRenderingWidget");// "QRenderingWidget (testing, with various stuff in layout, for testin.");
	poNewRenderingWidget->show();
	poNewRenderingWidget->StartRendering();

	return poNewRenderingWidget;
}

bool AddNewPrimaryAsset(QWidget* pQRenderWiget, QString* assetFolder, QByteArray* assetData, QString* outErrorString)
{
	auto renderWidget = static_cast<QtRenderWidgetView*>(pQRenderWiget);

	renderWidget->PauseRendering();
	auto gameIdString = renderWidget->GetGameIdString();
	auto currentScene = renderWidget->GetSceneManager()->GetCurrentScene();
	auto& resoureceManager = renderWidget->GetSceneManager()->GetResourceManager();

	try {

		ByteStream fileDataStream(assetData->data(), assetData->size(), assetFolder->toStdWString());
		rldx::DxSceneCreator::AddVariantMesh(rldx::DxDeviceManager::Device(), resoureceManager, currentScene, fileDataStream, gameIdString.toStdWString());
	}
	catch (std::exception& e)
	{
#ifdef _DEBUG
		MessageBoxA(reinterpret_cast<HWND>(pQRenderWiget->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
#endif
		* outErrorString = QString::fromStdString(std::string("Error: Excpetion: ") + e.what());

#ifdef _DEBUG
		LogAction(L"Error: Excpetion: " + utils::ToWString(e.what()));
#endif
		return false;
	}

	renderWidget->ResumeRendering();

	return true;
}

void SetAssetFolder(QString* folder)
{
	ByteStream::SetSearchFolder(folder->toStdWString());
}

void SetLogFolder(QString* folder)
{
	Logger::SetLogFolder(folder->toStdWString());
}

void PauseRendering(QWidget* pQRenderWiget)
{
	auto renderWidget = static_cast<QtRenderWidgetView*>(pQRenderWiget);
	renderWidget->PauseRendering();
}

void ResumeRendering(QWidget* pQRenderWiget)
{
	auto renderWidget = static_cast<QtRenderWidgetView*>(pQRenderWiget);
	renderWidget->ResumeRendering();
}

// TODO: remove, when everythig above works
//bool OLD_AddNewPrimaryAsset(QWidget* pQRenderWiget, QString* assetFolder, QByteArray* assetData, QString* outErrorString)
//{
//	auto renderWidget = static_cast<QtRenderWidgetView*>(pQRenderWiget);
//	auto gameIdString = renderWidget->GetGameIdString();
//	auto currentScene = renderWidget->GetSceneManager()->GetCurrentScene();
//
//	try {
//
//		ByteStream fileDataStream(assetData->data(), assetData->size(), assetFolder->toStdWString());
//		rldx::DxSceneCreator::AddModel(rldx::DxDeviceManager::Device(), currentScene, fileDataStream, gameIdString.toStdWString());
//	}
//	catch (std::exception& e)
//	{
//#ifdef _DEBUG
//		MessageBoxA(reinterpret_cast<HWND>(pQRenderWiget->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
//#endif
//
//		* outErrorString = QString::fromStdString(std::string("Error: Excpetion: ") + e.what());
//		LogAction(L"Error: Excpetion: " + utils::ToWString(e.what()));
//
//		return false;
//	}
//
//	return true;
//}

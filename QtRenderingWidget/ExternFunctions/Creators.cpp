#include "Creators.h"

#include <exception>

#include "..\..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"
#include "QtObjects\Views\QtRenderView.h"

#include "..\rldx\rldx\Logging\Logging.h"

void SetLogPath(const QString& path)
{
	// TODO: implement
	//throw std::exception("The method or operation is not implemented.");
}

void SetAssetPath(const QString& path)
{
	// TODO: implement
	//throw std::exception("The method or operation is not implemented.");
}

static int FileGetter(void* thisPtr, QVector<QString>* missingFiles, QVector<QByteArray>* outBinaries)
{
	int filesFound = 0;
	outBinaries->clear();
	outBinaries->resize(missingFiles->size());
	for (size_t iAsset = 0; iAsset < missingFiles->size(); iAsset++)
	{

		auto fileName = (*missingFiles)[iAsset].toStdWString();
		ByteStream newStream(fileName);

		if (!newStream.IsValid())
		{
			(*outBinaries)[iAsset].clear();
			continue;
		}
		
		// -- resize dest, buffer, and read whole file into it
		(*outBinaries)[iAsset].resize(newStream.GetBufferSize());		
		newStream.Read((*outBinaries)[iAsset].data(), (*outBinaries)[iAsset].size());
		filesFound++;
	}

	return filesFound;
}


QWidget* CreateQRenderingWidget(QWidget* parent, QString* gameIdString, AssetFetchCallBack pAssetCallBackFunc)
{

	rldx::DxResourceManager::SetcAssetFetchCallback(&FileGetter);
	// make sure the Singleton DeviceManager is instantiated
	//auto& spManager = rldx::DxDeviceManager::GetInstance();
	QtRenderView* poNewRenderingWidget = nullptr;

	try {
		poNewRenderingWidget = new QtRenderView(parent); // nullptr = no parent, free floating window	
		poNewRenderingWidget->hide();
		poNewRenderingWidget->Init();

	}
	catch (std::exception& e)
	{
		MessageBoxA(reinterpret_cast<HWND>(parent->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
		logging::LogAction(std::string("Error: Excpetion: ") + e.what());

		delete poNewRenderingWidget;
		return nullptr;
	}

	poNewRenderingWidget->setWindowFlag(Qt::WindowType::Widget);
	poNewRenderingWidget->setWindowTitle("BLAH");// "QRenderingWidget (testing, with various stuff in layout, for testin.");
	poNewRenderingWidget->show();
	poNewRenderingWidget->StartRendering(/*&spManager*/);

	return poNewRenderingWidget;

}

#include "QtTestAppView.h"

// Qt headers
#include <QDirIterator>
#include <qdockwidget.h>
#include <QFileInfo>
#include <qlayout.h>

#include "..\QtRenderingWidget\Constants\GameIdKeys.h"
#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include "..\Rldx\Rldx\Helpers\DxMeshCreatorHelper.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceByteStream.h"
#include "Utils\ByteStream.h"

#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"

// TODO: remove as many includes as possible
#include "..\ImportExport\FileFormats\Anim\Creators\AnimFileHeaderCreator.h"
#include "..\ImportExport\FileFormats\Anim\Reader\TwAnimReader.h"
#include "..\ImportExport\FileFormats\Anim\Reader\TwAnimReader.h"
#include "..\ImportExport\FileFormats\RigidModel\Readers\RigidModelReader.h"
#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include "..\Rldx\Rldx\Helpers\DxMeshCreatorHelper.h"
#include "..\Rldx\Rldx\Managers\DxDeviceManager.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceByteStream.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"
#include "TestData.h"

// TODO: REMOVE WHEN WORKING, make neater debugging stuff
#include <iostream>
#include "..\QtRenderingWidget\ExternFunctions\Creators.h" // TODO: change this, this is ugly, though I guess fine for testing.

QtMainWindowView::QtMainWindowView(QWidget* parent)
	: QMainWindow(parent)
{
	using namespace logging;
	Logger::LogActionSuccess(L"Starting the program.");
	setupUi(this);

	// TODO: maybe not have this constructor !!!! even for test program
	InitRenderView_DEBUG();

	setWindowTitle("TWViewer 0.0.1a - Total War Viewing/Conversion Tool");

	resize(1525, 1525);
}


QStringList DEBUG_GetAllFiles(const QString& path, const QString& extension)
{
	QFileInfoList list;
	QStringList listPaths;
	QDirIterator dirIt(path, QDirIterator::Subdirectories);
	while (dirIt.hasNext()) {
		dirIt.next();
		if (QFileInfo(dirIt.filePath()).isFile())
			if (QFileInfo(dirIt.filePath()).suffix() == extension)
			{
				list.append(QFileInfo(dirIt.filePath()));
				listPaths.push_back(QFileInfo(dirIt.filePath()).absoluteFilePath());

			}
	};

	return listPaths;
}

// TODO: REMOVE and cleanup
void QtMainWindowView::InitRenderView_DEBUG()
{
	// TODO: DOESN't WORK, get set to null, as the release version is called, make better debugging code, that doesn't affect the widget, maybe
	// Or really, the testerapp is not meant to run in Release anyway.
	auto instance = rldx::DxResourceManager::Instance(); // instantate "global" resource manager
	rldx::DxResourceManager::SetAssetFetchCallback(&DEBUG_Callback_FileGetter);

	auto ptestData = &test_app_data::testData_WH3_VMD_brt_ch_king_louen;
	auto ptestData2 = &test_app_data::testData_WH3_RMV2_Person_Malekith;

	auto qAssetPath = QString::fromStdWString(ptestData->assetFolder);

	rldx::DxResourceManager::SetGameAssetFolder(qAssetPath.toStdWString());
	QString gameIdString = QString::fromStdWString(ptestData->gameId);

	QString globalAssetFolder = QString::fromStdWString(LR"(I:\Coding\Repos\QtRenderingWidget\Rldx\Rldx\RenderResources\)");
	SetAssetFolder(&globalAssetFolder);

	QString globalLogFolder = QString::fromStdWString(LR"(c:\temp\)");
	SetLogFolder(&globalLogFolder);

	auto renderWidget1 = CreateQRenderingWidget(this, &gameIdString, nullptr, nullptr);
	if (!renderWidget1)	return;

	auto renderWidget2 = CreateQRenderingWidget(this, &gameIdString, nullptr, nullptr);
	if (!renderWidget2)	return;

	auto renderWidget3 = CreateQRenderingWidget(this, &gameIdString, nullptr, nullptr);
	if (!renderWidget2)	return;

	ByteStream bytes(ptestData->filePath);
	QString fileName = QString::fromStdWString(bytes.GetPath().c_str());
	QByteArray qBytes((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());
	QString outErrorString;



	ByteStream bytes2(ptestData2->filePath);
	QString fileName2 = QString::fromStdWString(bytes2.GetPath().c_str());
	QByteArray qBytes2((char*)bytes2.GetBufferPtr(), bytes2.GetBufferSize());
	QString outErrorString2;


	// testing if "refresh" works...(clearing node, reloading same asset)
	AddNewPrimaryAsset(renderWidget1, &fileName2, &qBytes2, &outErrorString2);

	AddNewPrimaryAsset(renderWidget2, &fileName, &qBytes, &outErrorString);
	AddNewPrimaryAsset(renderWidget3, &fileName, &qBytes, &outErrorString);


	setCentralWidget(renderWidget1); // add widget as the central widget in the QMainWindow 

	QDockWidget* dockWidget = new QDockWidget(tr("Dock Widget"), this);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
								Qt::RightDockWidgetArea);

	dockWidget->setWidget(renderWidget2);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget);



	dockWidget = new QDockWidget(tr("Dock Widget"), this);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
								Qt::RightDockWidgetArea);

	dockWidget->setWidget(renderWidget3);
	addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

QtMainWindowView::~QtMainWindowView()
{}

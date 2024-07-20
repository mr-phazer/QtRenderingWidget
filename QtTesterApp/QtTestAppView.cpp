#include "QtTestAppView.h"

// Qt headers
#include <QDirIterator>
#include <qdockwidget.h>
#include <QFileInfo>
#include <qlayout.h>

#include <CommonLibs\Utils\GameIdKeys.h>
#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include "..\Rldx\Rldx\Helpers\DxMeshCreatorHelper.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceByteStream.h"
#include <CommonLibs/Utils/ByteStream.h>

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
#include <qcoreapplication.h>
#include "..\QtRenderingWidget\ExternFunctions\Creators.h" // TODO: change this, this is ugly, though I guess fine for testing.

using namespace utils;

QtMainWindowView::QtMainWindowView(QWidget* parent)
	: QMainWindow(parent)
{
	logging::LogAction(L"Starting the program.");
	setupUi(this);

	// TODO: maybe not have this constructor !!!! even for test program
	InitRenderView_DEBUG();

	setWindowTitle("TWViewer 0.0.1a - Total War Viewing/Conversion Tool");

	resize(1525, 1525);
}


QtMainWindowView::~QtMainWindowView()
{}

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

	auto ptestData = &test_app_data::testData_WH3_VMD_brt_ch_king_louen;
	auto ptestData2 = &test_app_data::testData_WH3_WSMODEL_2;

	auto qAssetPath = QString::fromStdWString(ptestData->assetFolder);

	// TODO: move all the ugly stuff into a CONTROLLER, if this ever becomes a proper app
	rldx::DxResourceManager::SetGameAssetFolder(qAssetPath.toStdWString());
	QString gameIdString = QString::fromStdWString(ptestData->gameId);
	QString exeFolder = QCoreApplication::applicationDirPath();

	auto repoFolder = QString::fromStdWString(GetRepoRootFolderFromExe(exeFolder.toStdWString()));

	QString globalAssetFolder = repoFolder + QString::fromStdWString(LR"(/Rldx/Rldx/RenderResources/)");
	SetAssetFolder(&globalAssetFolder);

	QString globalLogFolder = repoFolder + QString::fromStdWString(LR"(/log/)");
	SetLogFolder(&globalLogFolder);

	m_renderWidget1 = CreateQRenderingWidget(this, &gameIdString, nullptr, nullptr);
	if (!m_renderWidget1)	return;

	auto renderWidget2 = CreateQRenderingWidget(this, &gameIdString, nullptr, nullptr);
	if (!renderWidget2)	return;

	auto renderWidget3 = CreateQRenderingWidget(this, &gameIdString, nullptr, nullptr);
	if (!renderWidget3)	return;

	ByteStream bytes(ptestData->filePath);
	QString fileName = QString::fromStdWString(bytes.GetPath().c_str());
	QByteArray qBytes((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());
	QString outErrorString;

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

	setCentralWidget(m_renderWidget1); // add widget as the central widget in the QMainWindow 
}

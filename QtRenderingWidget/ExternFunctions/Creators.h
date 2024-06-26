#pragma once

class QWidget;
class QString;
class Vector;

#include "Callbacks.h"

/// <summary>
/// Widget Creation, call the method to get a new Widget
/// </summary>
/// <param name="parent">Parent Qwidget, like  windows/layout</param>
/// <param name="gameIdString">Game string ID</param>
/// <param name="pAssetCallBackFunc">function pointer: call back for asset fetching</param>
/// <returns>Newly create Rendering Widget</returns>
extern "C" QWidget * CreateQRenderingWidget(
	QWidget * parent,
	QString * gameIdString,
	void (*AssetFetchCallBack) (QList<QString>*missingFiles, QList<QByteArray>*outBinFiles),
	void (*AnimPathsBySkeletonCallBack) (QString * skeletonName, QList<QString>*out) = nullptr
);

// TODO: remove?
//extern "C" QWidget * CreateQRenderingWidget(
//	QWidget * parent, 
//	uint32_t gameId, 
//	void (*AssetFetchCallBack) (QList<QString>*missingFiles, QList<QByteArray>*outBinFiles));



/// <summary>
/// Add an RigidModelV2, WSMODEL or VMD (model) to scene
/// TODO: WIP
/// </summary>
/// <param name="pQRendeeWiget">Thee rendering widget pointer</param>
/// <param name="assetFolder">internal/externala path</param>
/// <param name="assetData">raw binary data of asset</param>
extern "C" bool AddNewPrimaryAsset(QWidget * pQRenderWiget, QString * assetFolder, QByteArray * assetData, QString * outErrorString);

/// <summary>
/// Add an RigidModelV2, WSMODEL or VMD (model) to scene
/// TODO: WIP
/// </summary>
/// <param name="pQRendeeWiget">Thee rendering widget pointer</param>
/// <param name="assetFolder">internal/externala path</param>
/// <param name="assetData">raw binary data of asset</param>
extern "C" bool TESTCODE_AddNewPrimaryAsset(QWidget * pQRenderWiget, QString * assetFolder, QByteArray * assetData, QString * outErrorString);

/// <summary>
/// Path where shaders, cubemaps, and defalt material(textues) are stored
/// </summary>
/// <param name="parent"></param>
/// <param name="path"></param>
extern "C" void SetAssetFolder(QString * folder);
extern "C" void SetLogFolder(QString * folder);

/// <summary>
/// Sets the number of ms between each frame, FPS = 1000 / FrameTimeMS
/// </summary>
//extern "C" void SetFrameTime(QWidget * pQRendeeWiget, int FrameTimeMS);

extern "C" void PauseRendering(QWidget * pQRendeeWiget);
extern "C" void ResumeRendering(QWidget * pQRendeeWiget);







/// <summary>
/// Starts rendering, at certain FPS (optional)
/// </summary>
//extern "C" void StartRendering(QWidget * pQRendeeWiget, float framesPerSeconf = 60.0f);

/// <summary>
/// Pauses Rendering
/// </summary>
/// <param name="pQRendeeWiget"></param>
//extern "C" void PausetRendering(QWidget * pQRendeeWiget);

// for debugging, use local files as stand-in for packs
extern void DEBUG_Callback_FileGetter(QList<QString>* missingFiles, QList<QByteArray>* outBinFiles);
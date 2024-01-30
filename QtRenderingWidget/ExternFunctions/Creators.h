#pragma once

class QWidget;
class QString;
class Vector;


#include "Callbacks.h"

/// <summary>
/// Widget Creation, call the method to get a new Widget
/// </summary>
/// <param name="parent">Parent Qwidget, like  windows/layout</param>
/// <param name="gameIdString">your game string ID</param>
/// <param name="pAssetCallBackFunc">call back function for asset fetching</param>
/// <returns></returns>
extern "C" QWidget * CreateQRenderingWidget(QWidget * parent, QString* gameIdString, AssetFetchCallBack pAssetCallBackFunc);

/// <summary>
/// Add an RMV2, WSMODEL or VMD
/// </summary>
/// <param name="pQRendeeWiget">Thee rendereing widget pointer</param>
/// <param name="assetPath">internal/externala path</param>
/// <param name="assetData">raw binary data og asset</param>
extern "C" void AddNewPimaryAsset(QWidget * pQRendeeWiget, QString * assetPath, QByteArray* assetData);

/// <summary>
/// Path where shaders, cubemaps, and defalt material(textues) are stored
/// </summary>
/// <param name="parent"></param>
/// <param name="path"></param>
extern "C" void SetAssetPath(QWidget * pQRendeeWiget, const QString& path);

/// <summary>
/// Path where shaders, cubemaps, and defalt material(textues) are stored
/// </summary>
extern "C" void SetAssetLogPath(QWidget * pQRendeeWiget, const QString& path);

/// <summary>
/// Starts rendering, at certain FPS (optional)
/// </summary>
extern "C" void StartRendering(QWidget * pQRendeeWiget, float framesPerSeconf = 60.0f);

/// <summary>
/// Pauses Rendering
/// </summary>
/// <param name="pQRendeeWiget"></param>
extern "C" void PausetRendering(QWidget * pQRendeeWiget);


	
#pragma once


#include "QtWidgets\qwidget.h"
#include "QtCore\qvector.h"
#include "QtCore/qbytearray.h"
#include "QtCore/qstring.h"

/// <summary>
/// Callback function, for the Rendering Engine to call when it needs to load an asset
/// </summary>
/// <param name="missingFile"></param>
/// <param name="suppliedFiles"></param>
/// <returns>Number of files found and loade, 0 = error/Maybe negative values for error code</returns>
//typedef int (*AssetFetchCallBack) (void* thisPtr, QVector<QString>* missingFile, QVector<QByteArray>* suppliedFiles);

typedef void (*AssetFetchCallBack) (QList<QString>* missingFiles, QList<QByteArray>* outBinFiles);


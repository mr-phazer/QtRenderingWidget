#pragma once

#include <QByteArray>
#include <QList>
#include <QString>

/// <summary>
/// Callback function, for asset loading
/// </summary>
/// <param name="missingFiles">
/// List of files, or paths paths formated "somefolder/folder/" with terminating "/" </param>
/// <param name="outBinFiles">
/// all the files found, as binary pointers 
/// </param>
/// <returns></returns>
typedef void (*AssetFetchCallBack) (QList<QString>* missingFiles, QList<QByteArray>* outBinFiles);

#pragma once

#include <qbytearray.h>
#include <qlist.h>
#include <qstring.h>


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

typedef void (*AnimPathsBySkeletonCallBack) (QString* skeletonName, QList<QString>* out);
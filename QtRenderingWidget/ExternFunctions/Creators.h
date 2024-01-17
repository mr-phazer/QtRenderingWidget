#pragma once


class QWidget;
class QString;
extern "C" QWidget * CreateQRenderingWidget(QWidget * parent);

/// <summary>
/// For creating a QRenderingWidget with a file (RMV2/WSMODEL/VMD)
/// </summary>
/// <param name="parent">QWidget Parent</param>
/// <param name="pFilename">internal/external path name, used to id file type from extension</param>
/// <param name="fileData">binary file data</param>
/// <returns>Widget or null on error</returns>
//extern "C" QWidget * CreateQRenderingWidget(QWidget * parent, QString*pFilename, QByteArray*fileData);

extern "C" void SetLogPath(const QString& path);
extern "C" void SetAssetPath(const QString& path);

	
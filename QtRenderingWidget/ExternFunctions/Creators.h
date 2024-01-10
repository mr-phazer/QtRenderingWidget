#pragma once


class QWidget;
class QString;
extern "C" QWidget * CreateQRenderingWidget(QWidget * parent);

extern "C" void SetLogPath(const QString& path);
extern "C" void SetAssetPath(const QString& path);

	
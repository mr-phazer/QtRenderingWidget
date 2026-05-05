#pragma once

#include <qevent.h>
#include <qobject.h>
#include <qbytearray.h>
#include <qlist.h>
#include <qmimedata.h>
#include <qobjectdefs.h>
#include <qstring.h>
#include <qurl.h>
#include <Utils/ByteStream.h>

// -- Intentional circular dependency, as "controller" controls the "view", but also needs to be stored inside a QWdiget
class QtRenderWidgetView;

class QtRenderWidgetController : public QObject
{
	Q_OBJECT;

private:
	QtRenderWidgetView* view;

public:
	QtRenderWidgetController(QtRenderWidgetView* parentAndView);

	void ProcessMimeData(const QMimeData* mimeData);
	bool LoadNewAsset(QtRenderWidgetView* renderWidget, QString* assetFolder, QByteArray* assetData, QString* outErrorString);


public slots:
	void OnKeyPressed(QKeyEvent* keyEvent);
	void OnWindowClosing();
};


#pragma once

#include <qevent.h>
#include <qobject.h>


// -- Intentional circular dependency, as "controller" controls the "view", but also needs to be stored inside a QWdiget
class QtRenderWidgetView;

class QtRenderWidgetController : public QObject
{
	Q_OBJECT;

	QtRenderWidgetView* view;

public:
	QtRenderWidgetController(QtRenderWidgetView* parentAndView);

public slots:
	void OnKeyPressed(QKeyEvent* keyEvent);
	void OnWindowClosing();
};


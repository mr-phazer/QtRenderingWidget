#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtTesterApp.h"

class QtTesterApp : public QMainWindow, public Ui::QtTesterAppClass
{
    Q_OBJECT

public:
    QtTesterApp(QWidget *parent = nullptr);
    ~QtTesterApp();

private:
     
};

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtTesterApp.h"

class QtTesterAppWindow : public QMainWindow, public Ui::QtTesterAppClass
{
    Q_OBJECT

public:
    QtTesterAppWindow(QWidget *parent = nullptr);
    ~QtTesterAppWindow();

private:
     
};

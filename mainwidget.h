#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "panelconnect.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    PanelConnect* panel;
};

#endif // MAINWIDGET_H

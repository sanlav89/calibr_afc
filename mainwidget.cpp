#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    panel = new PanelConnect;
    panel->cmdGetVersion();
}

MainWidget::~MainWidget()
{

}

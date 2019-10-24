#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include "panelconnect.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    PanelConnect* panel;
    QPushButton* startBtn;
    QPushButton* getLastLogBtn;
    QRadioButton* ms40Rb;
    QRadioButton* ms80Rb;

private slots:
    void onStartBtn();
    void onGetLastLogBtn();
    void onMs40Rb(bool ms40);

};

#endif // MAINWIDGET_H

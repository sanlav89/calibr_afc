#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QTimer>
#include "panelconnect.h"
#include "calibrator2.h"

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
    QPushButton* startCalBtn;
    QPushButton* readCalBtn;
    QRadioButton* ms40Rb;
    QRadioButton* ms80Rb;
    QLineEdit* calCyclesLe;
    QTimer* timer;
    Calibrator2* calibrator;

private slots:
    void onStartBtn();
    void onGetLastLogBtn();
    void onStartCalBtn();
    void onTimeout();
    void onMs40Rb(bool ms40);
    void onReadCalBtn();
    void calAfcStatus(int cycles, bool done);
    void calAfcCalcAndSave(QByteArray data);

};

#endif // MAINWIDGET_H

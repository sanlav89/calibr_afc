#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QTimer>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QProgressBar>
#include "panelconnect.h"
#include "calibrator2.h"
#include "plotcalibr.h"
#include "helpers.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    PanelConnect* panel;
    QTimer* timer;
    Calibrator2* calibrator;
    QByteArray calData[2];

    QGroupBox* techModeGb;
    QPushButton* startTbModeBtn;
    QLabel* calCyclesLbl;
    QLineEdit* calCyclesLe;
    QLabel* mode4080Lbl;
    QRadioButton* ms40Rb;
    QRadioButton* ms80Rb;
    QPushButton* startCalBtn;
    QPushButton* readCalBtn;
    QPushButton* resetMprBtn;
//    QPushButton* getLastLogBtn;

    QGroupBox* graphicsGb;
    QLabel* mode4080Lbl2;
    QRadioButton* ms40Rb2;
    QRadioButton* ms80Rb2;
    QLabel* beamLbl;
    QSpinBox* beamSb;
    QPushButton* saveCalBtn;
    QPushButton* clearCalBtn;

    PlotCalibr* plotCalibr;
    QProgressBar* progressBar;
    QLabel* statusLbl;



    void initWidgetTechModeGb();
    void initWidgetGraphicsGb();
    void initWidgetOther();
    void initMainLayout();
    void initFunctionalModels();
    void performNoteToArpTable();

private slots:
    void onStartTbModeBtn();
    void onGetLastLogBtn();
    void onStartCalBtn();
    void onTimeout();
    void onMs40Rb(bool ms40);
    void onReadCalBtn();
    void calAfcStatus(int cycles, bool done);
    void calAfcCalcAndSave();
    void calAfcGetData(QByteArray data);
    void statusMessage(QString msg);

};

#endif // MAINWIDGET_H

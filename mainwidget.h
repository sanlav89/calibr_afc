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

#define DFF                 (1000000.0 / 39.0)      // Разрешение спектра

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    // Объекты формы для работы с МПР в технологическом режиме
    QPushButton* checkConnectBtn;
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
    // Объекты формы для расчета калибровки
    QGroupBox* graphicsGb;
    QLabel* mode4080Lbl2;
    QRadioButton* ms40Rb2;
    QRadioButton* ms80Rb2;
    QLabel* beamLbl;
    QSpinBox* beamSb;
    QPushButton* saveCalBtn;
    QPushButton* clearCalBtn;
    // Остальные объекты формы
    PlotCalibr* plotCalibr;
    QProgressBar* progressBar;
    QLabel* statusLbl;
    // Функциональные объекты
    PanelConnect* panel;
    QTimer* timer;
    Calibrator2* calibrator;
    QByteArray calData[2];
    // Инициализация объектов
    void initWidgetTechModeGb();
    void initWidgetGraphicsGb();
    void initWidgetOther();
    void initMainLayout();
    void initFunctionalModels();
    // Выполнение записи в ARP-таблицу
    void performNoteToArpTable();
    // Отображение кривых на графике
    void setGraphData(bool ms40, quint8 b_num);
    // Доступность элементов формы пользователю
    void setEnableWidgets(bool en1, bool en2, bool en3, bool en4,
                          bool en5, bool en6, bool en7, bool en8,
                          bool en9, bool en10, bool en11, bool en12);

private slots:
    void onCheckConnectBtn();
    void onStartTbModeBtn();
    void onGetLastLogBtn();
    void onStartCalBtn();
    void onTimeout();
    void onMs40Rb(bool ms40);
    void onMs80Rb(bool ms80);
    void onReadCalBtn();
    void calAfcStatus(int cycles, bool done);
    void calAfcCalcAndSave();
    void calAfcGetData(QByteArray data);
    void calAfcReadDataPart(quint16 partNum);
    void readPanelStatus(quint8);
    void updateGraphics();


};

#endif // MAINWIDGET_H

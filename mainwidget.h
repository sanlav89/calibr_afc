//==============================================================================
// (C) Copyright 2019 MIET
// Moscow, Zelenograd, Russia
//
// Device:      DISS
// Module:      MPR
// Component:   AFC calibration utility
// File:        mainwidget.h
// Function:    GUI class
// Notes:
// Author:      A.Lavrinenko
//==============================================================================
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

#define DFF (1000000.0 / 39.0)      // Разрешение спектра

//==============================================================================
/*
 * Класс GUI приложения
 */
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
    QPushButton* readErrorBtn;
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
    bool calDataReady[2];
    // Инициализация объектов GUI
    void initWidgetTechModeGb();
    void initWidgetGraphicsGb();
    void initWidgetOther();
    void initMainLayout();
    void initFunctionalModels();
    // Выполнение записи в ARP-таблицу
    void performNoteToArpTable();
    // Отображение данных на графике
    void setGraphData(bool ms40, quint8 b_num);
    // Доступность элементов GUI пользователю
    void setEnableWidgets(bool en1, bool en2, bool en3, bool en4,
                          bool en5, bool en6, bool en7, bool en8,
                          bool en9, bool en10, bool en11);
    // Произвести расчет поправочных коэффициентов АЧХ
    void calAfcCalc();

private slots:
    // Обработчики нажания на элементы GUI
    void onCheckConnectBtn();
    void onStartTbModeBtn();
    void onMs40Rb(bool ms40);
    void onMs80Rb(bool ms80);
    void onStartCalBtn();
    void onReadCalBtn();
    void onResetMprBtn();
    void onReadErrorBtn();
    void onSaveCalBtn();
    void onClearCalBtn();
    // Обработчик таймаута по таймеру
    void onTimeout();
    // Обработчик готовности ответа команды cmdCalAfcGetStatus()
    void calAfcStatus(int cycles, bool done);
    // Обработчик готовности ответа команды cmdCalAfcGetData()
    void calAfcReadDataPart(quint16 partNum);
    // Обработчик готовности данных, прочитанных из SRAM МПР
    void calAfcGetData(QByteArray data);
    // Обработчик слова состояния объекта "panel"
    void processPanelStatus(quint8);
    // Перерисовка графика
    void updateGraphics();
};
//==============================================================================

#endif // MAINWIDGET_H

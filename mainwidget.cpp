//==============================================================================
// (C) Copyright 2019 MIET
// Moscow, Zelenograd, Russia
//
// Device:      DISS
// Module:      MPR
// Component:   AFC calibration utility
// File:        mainwidget.cpp
// Function:    GUI class
// Notes:
// Author:      A.Lavrinenko
//==============================================================================
#include "mainwidget.h"
#include <QProcess>
#include <QGridLayout>
#include <QDebug>
#include <QFileDialog>

//==============================================================================
MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    // Инициализация объектов формы
    initWidgetTechModeGb();
    initWidgetGraphicsGb();
    initWidgetOther();
    initMainLayout();
    // Инициализация функциональных объектов
    initFunctionalModels();
    // Запись в ARP-таблицу
    performNoteToArpTable();
    // Проверка соединения
    onCheckConnectBtn();
}

MainWidget::~MainWidget()
{
}

/*
 * Инициализация объектов GUI
 */
void MainWidget::initWidgetTechModeGb()
{
    // Objects
    startTbModeBtn = new QPushButton("Запустить\n техн.-боевой\n режим");
    calCyclesLbl = new QLabel("Количество циклов:");
    calCyclesLe = new QLineEdit("50");
    mode4080Lbl = new QLabel("Режим:");
    ms40Rb = new QRadioButton("40 мс");
    ms80Rb = new QRadioButton("80 мс");
    startCalBtn = new QPushButton("Запустить\n режим\n калибровки");
    readCalBtn = new QPushButton("Прочитать\n память");
    resetMprBtn = new QPushButton("Сбросить\n МПР");
    checkConnectBtn = new QPushButton("Проверить\n соединение");
    readErrorBtn =  new QPushButton("Прочитать\n код ошибки");
    // Init Object's Properties
    startTbModeBtn->setEnabled(false);
    calCyclesLe->setEnabled(false);
    ms80Rb->setEnabled(false);
    ms40Rb->setEnabled(false);
    startCalBtn->setEnabled(false);
    readCalBtn->setEnabled(false);
    resetMprBtn->setEnabled(true);

    // Init Object's Locations
    techModeGb = new QGroupBox(tr("Технологический режим"));
    QGridLayout *techModeLayout = new QGridLayout(techModeGb);
    techModeLayout->addWidget(checkConnectBtn,  0, 0, 1, 2);
    techModeLayout->addWidget(startTbModeBtn,   1, 0, 1, 2);
    techModeLayout->addWidget(mode4080Lbl,      2, 0, 1, 2);
    techModeLayout->addWidget(ms40Rb,           3, 0, 1, 1);
    techModeLayout->addWidget(ms80Rb,           3, 1, 1, 1);
    techModeLayout->addWidget(calCyclesLbl,     4, 0, 1, 2);
    techModeLayout->addWidget(calCyclesLe,      5, 0, 1, 2);
    techModeLayout->addWidget(startCalBtn,      6, 0, 1, 2);
    techModeLayout->addWidget(readCalBtn,       7, 0, 1, 2);
    techModeLayout->addWidget(resetMprBtn,      8, 0, 1, 2);
    techModeLayout->addWidget(readErrorBtn,     9, 0, 1, 2);
    // Init Object's connections
    connect(checkConnectBtn, SIGNAL(clicked()),
            this, SLOT(onCheckConnectBtn()));
    connect(startTbModeBtn, SIGNAL(clicked()), this, SLOT(onStartTbModeBtn()));
    connect(startCalBtn, SIGNAL(clicked()), this, SLOT(onStartCalBtn()));
    connect(readCalBtn, SIGNAL(clicked()), this, SLOT(onReadCalBtn()));
    connect(ms40Rb, SIGNAL(toggled(bool)), this, SLOT(onMs40Rb(bool)));
    connect(ms80Rb, SIGNAL(toggled(bool)), this, SLOT(onMs80Rb(bool)));
    connect(resetMprBtn, SIGNAL(clicked()), this, SLOT(onResetMprBtn()));
    connect(readErrorBtn, SIGNAL(clicked()), this, SLOT(onReadErrorBtn()));
}

void MainWidget::initWidgetGraphicsGb()
{
    // Objects
    mode4080Lbl2 = new QLabel("Режим:");
    ms40Rb2 = new QRadioButton("40 мс");
    ms80Rb2 = new QRadioButton("80 мс");
    beamLbl = new QLabel("Луч:");
    beamSb = new QSpinBox;
    saveCalBtn = new QPushButton("Сохранить\n калибровку");
    clearCalBtn = new QPushButton("Очистить\n калибровку");
    // Init Object's Properties
    ms40Rb2->setEnabled(false);
    ms40Rb2->setChecked(true);
    ms80Rb2->setEnabled(false);
    beamSb->setRange(1, 4);
    beamSb->setEnabled(false);
    saveCalBtn->setEnabled(false);
    // Init Object's Locations
    graphicsGb = new QGroupBox(tr("Анализ АЧХ"));
    QGridLayout *graphicsLayout = new QGridLayout(graphicsGb);
    graphicsLayout->addWidget(mode4080Lbl2, 0, 0, 1, 2);
    graphicsLayout->addWidget(ms40Rb2,      1, 0, 1, 1);
    graphicsLayout->addWidget(ms80Rb2,      1, 1, 1, 1);
    graphicsLayout->addWidget(beamLbl,      2, 0, 1, 1);
    graphicsLayout->addWidget(beamSb,       2, 1, 1, 1);
    graphicsLayout->addWidget(saveCalBtn,   3, 0, 1, 2);
    graphicsLayout->addWidget(clearCalBtn,  4, 0, 1, 2);
    // Init Object's connections
    connect(saveCalBtn, SIGNAL(clicked()), this, SLOT(onSaveCalBtn()));
    connect(beamSb, SIGNAL(valueChanged(int)), this, SLOT(updateGraphics()));
    connect(ms40Rb2, SIGNAL(toggled(bool)), this, SLOT(updateGraphics()));
    connect(clearCalBtn, SIGNAL(clicked()), this, SLOT(onClearCalBtn()));
}

void MainWidget::initWidgetOther()
{
    // Objects
    plotCalibr = new PlotCalibr(
                "АЧХ",
                "Частота [Гц]",
                "Амплитуда [дБ]",
                QColor(75, 75, 75),
                QColor(25, 25, 25));
    progressBar = new QProgressBar;
    statusLbl = new QLabel("Подключение...");
    // Init Object's Properties
    plotCalibr->setMinimumWidth(800);
    statusLbl->setFont(QFont("Helvetica", 8, QFont::Bold));
}

void MainWidget::initMainLayout()
{
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(techModeGb,  0, 0, 10, 1);
    mainLayout->addWidget(graphicsGb,  10, 0, 6, 1);
    mainLayout->addWidget(plotCalibr,  0, 1, 16, 9);
    mainLayout->addWidget(progressBar, 16, 0, 1, 10);
    mainLayout->addWidget(statusLbl,   17, 0, 1, 10);
    setLayout(mainLayout);
    setWindowTitle("Калибровка АЧХ");
}

void MainWidget::initFunctionalModels()
{
    // Objects
    panel = new PanelConnect;
    calibrator = new Calibrator2;
    timer = new QTimer;
    // Init Object's Properties
    for (int i = 0; i < 2; i++) {
        calData[i].resize(32768);
    }
    onClearCalBtn();

    // Init Object's connections
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(panel, SIGNAL(cmdCalAfcStatusReady(int, bool)),
            this, SLOT(calAfcStatus(int, bool)));
    connect(panel, SIGNAL(cmdCalAfcDataReady(QByteArray)),
            this, SLOT(calAfcGetData(QByteArray)));
    connect(panel, SIGNAL(panelStatus(quint8)),
            this, SLOT(processPanelStatus(quint8)));
    connect(panel, SIGNAL(cmdCalAfcGetDataPartReady(quint16)),
            this, SLOT(calAfcReadDataPart(quint16)));
}

/*
 * Выполнение записи в ARP-таблицу
 */
void MainWidget::performNoteToArpTable()
{
// Запись в ARP-таблицу соответствия IP-адреса ДИСС с MAC-адресом
    QProcess *prog = new QProcess(this);
    prog->start("arp",
                QString("-s 192.168.1.163 12-34-56-78-9a-bc 192.168.1.160")
                .split(" "));
    if (!prog->waitForFinished())
        qDebug() << false;
    else
        qDebug() << true;
    qDebug() << prog->readAllStandardOutput();
    qDebug() << prog->readAllStandardError();
    prog->deleteLater();
}

/*
 * Отображение данных на графике
 */
void MainWidget::setGraphData(bool ms40, quint8 b_num)
{
    double freqscale[1024];
    double dataY[2][1024];
    double F = Calibrator2::DFF / (2 - (int)ms40);
    double* pSpecData = calibrator->GetSrcSpectrums((quint8)ms40, b_num);
    double* pCompData = calibrator->GetCompAfc((quint8)ms40, b_num);
    double ymin, ymax1, ymax2, xmin = 0, xmax = 0;

    ymin = 10 * log10(pSpecData[Calibrator2::CUT_AFC_POS]
            * pCompData[Calibrator2::CUT_AFC_POS] / 2);
    ymax1 = 10 * log10(pSpecData[Calibrator2::CUT_AFC_POS]);
    ymax2 = ymin;
    for (int i = 0; i < 1024; i++) {
        freqscale[i] = -F / 2 + i * F / 1024;
        dataY[0][i] = 10 * log10(pSpecData[i]);
        dataY[1][i] = 10 * log10(pSpecData[i] * pCompData[i] / 2);
        if (i >= Calibrator2::CUT_AFC_POS && i < Calibrator2::FFT_LENGTH -
                Calibrator2::CUT_AFC_POS) {
            if (dataY[0][i] > ymax1) {
                ymax1 = dataY[0][i];
            }
            if (dataY[1][i] < ymin) {
                xmin = freqscale[i];
                ymin = dataY[1][i];
            }
            if (dataY[1][i] > ymax2) {
                xmax = freqscale[i];
                ymax2 = dataY[1][i];
            }
        }
    }
    plotCalibr->UpdateCurves(freqscale, dataY, xmin, ymin, xmax, ymax2);
    plotCalibr->SetScale(-F / 2, F / 2, ymin - 1, ymax1 + 1);
}

/*
 * Доступность элементов GUI пользователю
 */
void MainWidget::setEnableWidgets(
        bool en1, bool en2, bool en3, bool en4,
        bool en5, bool en6, bool en7, bool en8,
        bool en9, bool en10, bool en11
        )
{
    checkConnectBtn->setEnabled(en1);
    startTbModeBtn->setEnabled(en2);
    calCyclesLe->setEnabled(en3);
    ms80Rb->setEnabled(en4);
    ms40Rb->setEnabled(en5);
    startCalBtn->setEnabled(en6);
    readCalBtn->setEnabled(en7);
    resetMprBtn->setEnabled(en8);
    ms40Rb2->setEnabled(en9);
    ms80Rb2->setEnabled(en10);
    beamSb->setEnabled(en11);
}

/*
 * Произвести расчет поправочных коэффициентов АЧХ
 */
void MainWidget::calAfcCalc()
{
    QByteArray data;
    data.append(calData[0]);
    data.append(calData[1]);
    saveCalBtn->setEnabled(calDataReady[0] && calDataReady[1]);
    if (data.size() == 65536) {
        qDebug() << "Calibration calculating is started...";
        calibrator->Calibrate(data, calCyclesLe->text().toInt());
        updateGraphics();
    }
    else {
        qDebug() << "Error! Wrong Data Size: " << data.size();
    }
}

/*
 * Обработчики нажания на элементы GUI
 */
void MainWidget::onCheckConnectBtn()
{
    progressBar->setValue(0);
    ms40Rb->clearFocus();
    if (ms40Rb->isChecked()) {
        ms40Rb->setChecked(false);
    }
    processPanelStatus(PanelConnect::ST_CONNECT_FAIL);
    panel->cmdGetVersion();
}

void MainWidget::onStartTbModeBtn()
{
    panel->cmdMainModeSetParams();
}

void MainWidget::onMs40Rb(bool ms40)
{
    panel->cmdMainModeSetFpga4080(ms40);
}

void MainWidget::onMs80Rb(bool ms80)
{
    panel->cmdMainModeSetFpga4080(!ms80);
}

void MainWidget::onStartCalBtn()
{
    panel->cmdCalAfcSetCtrl(calCyclesLe->text().toInt());
    progressBar->setRange(0, calCyclesLe->text().toInt());
    timer->start(500);
}

void MainWidget::onReadCalBtn()
{
    panel->cmdCalAfcGetData(0);
    progressBar->setRange(0, 511);
    timer->start(500);
}

void MainWidget::onResetMprBtn()
{
    panel->cmdProgramResetMpr();
}

void MainWidget::onReadErrorBtn()
{
    panel->cmdGetLastLog();
}

void MainWidget::onSaveCalBtn()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(
                this,
                tr("Сохранить файл поправок"),
                QDir::current().absolutePath(),
                tr("TXT (*.txt)"));
    if (!filename.isNull()) {
        calibrator->SaveCalibration(filename);
        qDebug() << "Calibration is saved";
    }
}

void MainWidget::onClearCalBtn()
{

    for (int i = 0; i < 2; i++) {
        memset(calData[i].data(), 0, calData[i].size());
        calDataReady[i] = false;
    }
    calAfcCalc();
}

/*
 * Обработчик таймаута по таймеру (timer)
 */
void MainWidget::onTimeout()
{
    switch (panel->getStatus()) {
    case PanelConnect::ST_ACCUM_CALIBR_PERFOMING:
        panel->cmdCalAfcGetStatus();
        break;
    case PanelConnect::ST_READING_DATA_PERFOMING:
        panel->cmdCalAfcGetDataRepeat();
        break;
    }
}

/*
 * Обработчик готовности ответа команды cmdCalAfcGetStatus()
 */
void MainWidget::calAfcStatus(int cycles, bool done)
{
    qDebug("Cycles %d / %d, done: %d",
           cycles, calCyclesLe->text().toInt(), done);
    progressBar->setValue(cycles);
    if (done) {
        progressBar->setValue(calCyclesLe->text().toInt());
        timer->stop();
        qDebug() << "Calibration spectrums are ready...";
    }
}

/*
 * Обработчик готовности ответа команды cmdCalAfcGetData(partNum)
 */
void MainWidget::calAfcReadDataPart(quint16 partNum)
{
    progressBar->setValue(partNum);
}

/*
 *  Обработчик готовности всех данных, прочитанных из SRAM МПР
 */
void MainWidget::calAfcGetData(QByteArray data)
{
    if (ms40Rb->isChecked()) {
        calData[1] = data;
        calDataReady[1] = true;
    }
    if (ms80Rb->isChecked()) {
        calData[0] = data;
        calDataReady[0] = true;
    }
}

/*
 * Обработчик слова состояния объекта "panel"
 */
void MainWidget::processPanelStatus(quint8 status)
{
    QString statusMsg;
    QPalette pal;
    switch (status) {
    case PanelConnect::ST_CONNECT_FAIL:
        statusMsg.append("ОШИБКА! Нет подключения!");
        pal.setColor(QPalette::WindowText, Qt::darkRed);
        setEnableWidgets(true, false, false, false, false, false, false,
                         true, true, true, true);
        break;
    case PanelConnect::ST_CONNECT_READY:
        statusMsg.append("Подключено. ");
        pal.setColor(QPalette::WindowText, Qt::darkGreen);
        setEnableWidgets(true, true, false, false, false, false, false,
                         true, true, true, true);
        break;
    case PanelConnect::ST_READY_TO_SET_4080MS:
        statusMsg.append("Подключено. Техн.-боевой режим МПР. "
                         "Выберите режим: 40 или 80 мс");
        pal.setColor(QPalette::WindowText, Qt::darkGreen);
        setEnableWidgets(true, false, false, true, true, false, false,
                         true, true, true, true);
        if (ms40Rb->isChecked() || ms80Rb->isChecked())
            onMs40Rb(ms40Rb->isChecked());
        break;
    case PanelConnect::ST_READY_TO_START_CALIBR:
        statusMsg.sprintf("Подключено. Техн.-боевой режим МПР. "
                          "Выбран режим %d мс. Введите количество "
                          "циклов и запустите процесс калибровки.",
                          80 - 40 * (int)(ms40Rb->isChecked()));
        pal.setColor(QPalette::WindowText, Qt::darkGreen);
        setEnableWidgets(true, false, true, true, true, true, true,
                         true, true, true, true);
        break;
    case PanelConnect::ST_ACCUM_CALIBR_PERFOMING:
        statusMsg.append("Подключено. Техн.-боевой режим МПР. "
                         "Выполняется процесс суммирования спектров...");
        pal.setColor(QPalette::WindowText, Qt::darkGreen);
        setEnableWidgets(false, false, false, false, false, false, false,
                         true, true, true, true);
        break;
    case PanelConnect::ST_READY_TO_READ_CALIBR_DATA:
        statusMsg.append("Подключено. Техн.-боевой режим МПР. "
                         "Процесс суммирования спектров завершен. Данные готовы"
                         " для чтения.");
        pal.setColor(QPalette::WindowText, Qt::darkGreen);
        setEnableWidgets(true, false, false, false, false, false, true,
                         true, true, true, true);
        break;
    case PanelConnect::ST_READING_DATA_PERFOMING:
        statusMsg.append("Подключено. Техн.-боевой режим МПР. "
                         "Выполняется процесс чтения спектров...");
        pal.setColor(QPalette::WindowText, Qt::darkGreen);
        setEnableWidgets(false, false, false, false, false, false, false,
                         true, true, true, true);
        break;
    case PanelConnect::ST_READING_DATA_DONE:
        timer->stop();
        calAfcCalc();
        statusMsg.append("Подключено. Техн.-боевой режим МПР. "
                         "Чтение спектров завершено. Проведите анализ и "
                         "сохраните поправочные характеристики АЧХ.");
        pal.setColor(QPalette::WindowText, Qt::darkGreen);
        setEnableWidgets(true, false, true, true, true, true, true,
                         true, true, true, true);
        break;
    case PanelConnect::ST_TELEM_MODE:
        statusMsg.append("Подключено. МПР в режиме телеметрии. Сбросьте МПР, "
                         "чтобы продолжить");
        pal.setColor(QPalette::WindowText, Qt::darkBlue);
        setEnableWidgets(true, false, false, false, false, false, false,
                         true, true, true, true);
        break;
    case PanelConnect::ST_LAST_LOG_ERR:
        statusMsg.sprintf("ОШИБКА! МПР в Отказе. Код ошибки: 0x%20X. "
                          "Сбросьте МПР, чтобы продолжить", panel->getLogErr());
        pal.setColor(QPalette::WindowText, Qt::darkRed);
        setEnableWidgets(true, false, false, false, false, false, false,
                         true, true, true, true);
        break;
    case PanelConnect::ST_LAST_LOG_SUCC:
        statusMsg.append("Подключено. Все в порядке, отказа нет.");
        pal.setColor(QPalette::WindowText, Qt::darkGreen);
//        setEnableWidgets(true, false, false, false, false, false, false,
//                         true, true, true, true, true);
        break;
    }
    statusLbl->setPalette(pal);
    statusLbl->setText(statusMsg);
}

/*
 * Перерисовка графика
 */
void MainWidget::updateGraphics()
{
    setGraphData(ms40Rb2->isChecked(), beamSb->value() - 1);
}
//==============================================================================

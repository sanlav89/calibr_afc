#include "mainwidget.h"
#include <QProcess>
#include <QGridLayout>
#include <QDebug>

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
    // Установка размера данных джля выполнения калибровки
    calData[0].resize(32768);
    calData[1].resize(32768);
    // Проверка соединения
    onCheckConnectBtn();
}

MainWidget::~MainWidget()
{
}

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
    // Init Object's connections
    connect(checkConnectBtn, SIGNAL(clicked()),
            this, SLOT(onCheckConnectBtn()));
    connect(startTbModeBtn, SIGNAL(clicked()), this, SLOT(onStartTbModeBtn()));
    connect(startCalBtn, SIGNAL(clicked()), this, SLOT(onStartCalBtn()));
    connect(readCalBtn, SIGNAL(clicked()), this, SLOT(onReadCalBtn()));
    connect(ms40Rb, SIGNAL(toggled(bool)), this, SLOT(onMs40Rb(bool)));
    connect(ms80Rb, SIGNAL(toggled(bool)), this, SLOT(onMs80Rb(bool)));
    connect(resetMprBtn, SIGNAL(clicked()), this, SLOT(onResetMprBtn()));
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
    // Init Object's Properties
    ms40Rb2->setEnabled(false);
    ms40Rb2->setChecked(true);
    ms80Rb2->setEnabled(false);
    beamSb->setRange(1, 4);
    beamSb->setEnabled(false);
    saveCalBtn->setEnabled(false);
    // Init Object's Locations
    graphicsGb = new QGroupBox(tr("Расчет / Графики"));
    QGridLayout *graphicsLayout = new QGridLayout(graphicsGb);
    graphicsLayout->addWidget(mode4080Lbl2, 0, 0, 1, 2);
    graphicsLayout->addWidget(ms40Rb2,      1, 0, 1, 1);
    graphicsLayout->addWidget(ms80Rb2,      1, 1, 1, 1);
    graphicsLayout->addWidget(beamLbl,      2, 0, 1, 1);
    graphicsLayout->addWidget(beamSb,       2, 1, 1, 1);
    graphicsLayout->addWidget(saveCalBtn,   3, 0, 1, 2);
    // Init Object's connections
    connect(saveCalBtn, SIGNAL(clicked()), this, SLOT(onSaveCalBtn()));
    connect(beamSb, SIGNAL(valueChanged(int)), this, SLOT(updateGraphics()));
    connect(ms40Rb2, SIGNAL(toggled(bool)), this, SLOT(updateGraphics()));
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
    plotCalibr->setMinimumWidth(700);
    // Init Object's Locations

    // Init Object's connections

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

    // Init Object's connections
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(panel, SIGNAL(cmdCalAfcStatusReady(int, bool)),
            this, SLOT(calAfcStatus(int, bool)));
    connect(panel, SIGNAL(cmdCalAfcDataReady(QByteArray)),
            this, SLOT(calAfcGetData(QByteArray)));
    connect(panel, SIGNAL(panelStatus(quint8)),
            this, SLOT(readPanelStatus(quint8)));
    connect(panel, SIGNAL(cmdCalAfcGetDataPartReady(quint16)),
            this, SLOT(calAfcReadDataPart(quint16)));
    connect(panel, SIGNAL(cmdCalAfcGetDataPartReady(quint16)),
            this, SLOT(calAfcReadDataPart(quint16)));
}

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

void MainWidget::onCheckConnectBtn()
{
    progressBar->setValue(0);
    ms40Rb->clearFocus();
    if (ms40Rb->isChecked()) {
        qDebug() << "checked";
        ms40Rb->setChecked(false);
    }

//    else if (ms80Rb->isChecked())
//        ms80Rb->setChecked(true);
    readPanelStatus(ST_CONNECT_FAIL);
    panel->cmdGetVersion();
    qDebug() << "Check connect";
}

void MainWidget::onStartTbModeBtn()
{
//    panel->cmdMainModeStart();
    panel->cmdMainModeSetParams();
}

void MainWidget::onGetLastLogBtn()
{
    panel->cmdGetLastLog();
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

void MainWidget::onTimeout()
{
    switch (panel->getStatus()) {
    case ST_ACCUM_CALIBR_PERFOMING:
        panel->cmdCalAfcGetStatus();
        break;
    case ST_READING_DATA_PERFOMING:
        panel->cmdCalAfcGetDataRepeat();
        break;
    }
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

void MainWidget::calAfcCalc()
{
    QByteArray data;
    data.append(calData[0]);
    data.append(calData[1]);
    if (data.size() == 65536) {
        qDebug() << "Calibration calculating is started...";
        calibrator->Calibrate(data, calCyclesLe->text().toInt());
    }
    else {
        qDebug() << "Error! Wrong Data Size: " << data.size();
    }
}

void MainWidget::onSaveCalBtn()
{
    calibrator->SaveCalibration();
    qDebug() << "Calibration is saved";
}

void MainWidget::calAfcGetData(QByteArray data)
{
    if (ms40Rb->isChecked())
        calData[1] = data;
    if (ms80Rb->isChecked())
        calData[0] = data;
}

void MainWidget::calAfcReadDataPart(quint16 partNum)
{
    progressBar->setValue(partNum);
}


void MainWidget::readPanelStatus(quint8 status)
{
    QString statusMsg;
    switch (status) {
    case ST_CONNECT_FAIL:
        statusMsg.append("ОШИБКА! Нет подключения!");
        setEnableWidgets(true, false, false, false, false, false, false,
                         true, true, true, true, true);
        break;
    case ST_CONNECT_READY:
        statusMsg.append("Подключение есть. ");
        setEnableWidgets(true, true, false, false, false, false, false,
                         true, true, true, true, true);
        break;
    case ST_READY_TO_SET_4080MS:
        statusMsg.append("Подключение есть. Техн.-боевой режим МПР. "
                         "Выберите режим: 40 или 80 мс");
        setEnableWidgets(true, false, false, true, true, false, false,
                         true, true, true, true, true);
        if (ms40Rb->isChecked() || ms80Rb->isChecked())
            onMs40Rb(ms40Rb->isChecked());
        break;
    case ST_READY_TO_START_CALIBR:
        statusMsg.sprintf("Подключение есть. Техн.-боевой режим МПР. "
                          "Выбран режим %d мс. Введите количество "
                          "циклов и запустите процесс калибровки.",
                          80 - 40 * (int)(ms40Rb->isChecked()));
        setEnableWidgets(true, false, true, true, true, true, true,
                         true, true, true, true, true);
        break;
    case ST_ACCUM_CALIBR_PERFOMING:
        statusMsg.append("Подключение есть. Техн.-боевой режим МПР. "
                         "Выполняется процесс суммирования спектров...");
        setEnableWidgets(false, false, false, false, false, false, false,
                         true, true, true, true, true);
        break;
    case ST_READY_TO_READ_CALIBR_DATA:
        statusMsg.append("Подключение есть. Техн.-боевой режим МПР. "
                         "Процесс суммирования спектров завершен. Данные готовы"
                         " для чтения");
        setEnableWidgets(true, false, false, false, false, false, true,
                         true, true, true, true, true);
        break;
    case ST_READING_DATA_PERFOMING:
        statusMsg.append("Подключение есть. Техн.-боевой режим МПР. "
                         "Выполняется процесс чтения спектров...");
        setEnableWidgets(false, false, false, false, false, false, false,
                         true, true, true, true, true);
        break;
    case ST_READING_DATA_DONE:
        timer->stop();
        calAfcCalc();
        updateGraphics();
        statusMsg.append("Подключение есть. Техн.-боевой режим МПР. "
                         "Чтение спектров завершено. Проведите анализ и "
                         "сохраните поправочные характеристики АЧХ.");
        setEnableWidgets(true, false, true, true, true, true, true,
                         true, true, true, true, true);
        break;
    case ST_TELEM_MODE:
        statusMsg.append("МПР в режиме телеметрии. Сбросьте МПР, чтобы "
                         "продолжить");
        setEnableWidgets(true, false, false, false, false, false, false,
                         true, true, true, true, true);
        break;
    }

    statusLbl->setText(statusMsg);
}

void MainWidget::setGraphData(bool ms40, quint8 b_num)
{
    double freqscale[1024];
    double dataY[2][1024];
    double F = DFF / (2 - (int)ms40);
    double* pSpecData = calibrator->GetSrcSpectrums((quint8)ms40, b_num);
    double* pCompData = calibrator->GetCompAfc((quint8)ms40, b_num);
    double ymin, ymax1, ymax2, xmin = 0, xmax = 0;

    ymin = 10 * log10(pSpecData[CUT_AFC_POS] * pCompData[CUT_AFC_POS] / 2);
    ymax1 = 10 * log10(pSpecData[CUT_AFC_POS]);
    ymax2 = ymin;
    for (int i = 0; i < 1024; i++) {
        freqscale[i] = -F / 2 + i * F / 1024;
        dataY[0][i] = 10 * log10(pSpecData[i]);
        dataY[1][i] = 10 * log10(pSpecData[i] * pCompData[i] / 2);
        if (i >= CUT_AFC_POS && i < FFT_LENGTH - CUT_AFC_POS) {
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

void MainWidget::updateGraphics()
{
    setGraphData(ms40Rb2->isChecked(), beamSb->value() - 1);
}

void MainWidget::setEnableWidgets(
        bool en1, bool en2, bool en3, bool en4,
        bool en5, bool en6, bool en7, bool en8,
        bool en9, bool en10, bool en11, bool en12
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
    saveCalBtn->setEnabled(en12);
}

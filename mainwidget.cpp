#include "mainwidget.h"
#include <QProcess>
#include <QGridLayout>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    initWidgetTechModeGb();
    initWidgetGraphicsGb();
    initWidgetOther();
    initMainLayout();
    initFunctionalModels();
    performNoteToArpTable();
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
    // Init Object's Properties

    // Init Object's Locations
    techModeGb = new QGroupBox(tr("Технологический режим"));
    QGridLayout *techModeLayout = new QGridLayout(techModeGb);
    techModeLayout->addWidget(startTbModeBtn,   0, 0, 1, 2);
    techModeLayout->addWidget(calCyclesLbl,     1, 0, 1, 2);
    techModeLayout->addWidget(calCyclesLe,      2, 0, 1, 2);
    techModeLayout->addWidget(mode4080Lbl,      3, 0, 1, 2);
    techModeLayout->addWidget(ms40Rb,           4, 0, 1, 1);
    techModeLayout->addWidget(ms80Rb,           4, 1, 1, 1);
    techModeLayout->addWidget(startCalBtn,      5, 0, 1, 2);
    techModeLayout->addWidget(readCalBtn,       6, 0, 1, 2);
    techModeLayout->addWidget(resetMprBtn,      7, 0, 1, 2);
    // Init Object's connections
    connect(startTbModeBtn, SIGNAL(clicked()), this, SLOT(onStartTbModeBtn()));
    connect(startCalBtn, SIGNAL(clicked()), this, SLOT(onStartCalBtn()));
    connect(readCalBtn, SIGNAL(clicked()), this, SLOT(onReadCalBtn()));
    connect(ms40Rb, SIGNAL(toggled(bool)), this, SLOT(onMs40Rb(bool)));
//    connect(getLastLogBtn, SIGNAL(clicked()), this, SLOT(onGetLastLogBtn()));
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
    beamSb->setRange(1, 4);
    // Init Object's Locations
    graphicsGb = new QGroupBox(tr("Расчет / Графики"));
    QGridLayout *graphicsLayout = new QGridLayout(graphicsGb);
    graphicsLayout->addWidget(mode4080Lbl2, 0, 0, 1, 2);
    graphicsLayout->addWidget(ms40Rb2,      1, 0, 1, 1);
    graphicsLayout->addWidget(ms80Rb2,      1, 1, 1, 1);
    graphicsLayout->addWidget(beamLbl,      2, 0, 1, 1);
    graphicsLayout->addWidget(beamSb,       2, 1, 1, 1);
    graphicsLayout->addWidget(saveCalBtn,   3, 0, 1, 2);
    graphicsLayout->addWidget(clearCalBtn,  4, 0, 1, 2);
    // Init Object's connections
//    connect(calcAfcCalcBtn, SIGNAL(clicked()), this, SLOT(calAfcCalcAndSave()));
}

void MainWidget::initWidgetOther()
{
    // Objects
    plotCalibr = new PlotCalibr(
                "АЧХ",
                "Частота [Гц]",
                "Амплитуда [дБ]",
                QColor(125, 125, 125),
                QColor(75, 75, 75));
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
    mainLayout->addWidget(techModeGb,  0, 0, 1, 1);
    mainLayout->addWidget(graphicsGb,  1, 0, 1, 1);
    mainLayout->addWidget(plotCalibr,  0, 1, 2, 9);
    mainLayout->addWidget(progressBar, 2, 0, 1, 10);
    mainLayout->addWidget(statusLbl,   3, 0, 1, 10);
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
    panel->cmdGetVersion();
    // Init Object's connections
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(panel, SIGNAL(cmdCalAfcStatusReady(int, bool)),
            this, SLOT(calAfcStatus(int, bool)));
    connect(panel, SIGNAL(cmdCalAfcDataReady(QByteArray)),
            this, SLOT(calAfcGetData(QByteArray)));
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

void MainWidget::onStartCalBtn()
{
    if (ms40Rb->isChecked())
        calData[1].resize(0);
    if (ms80Rb->isChecked())
        calData[0].resize(0);
    panel->cmdCalAfcSetCtrlGetStatus(calCyclesLe->text().toInt(), false);
    timer->start(500);

}

void MainWidget::onTimeout()
{
    panel->cmdCalAfcSetCtrlGetStatus(0, true);
}

void MainWidget::onReadCalBtn()
{
    panel->cmdCalAfcGetData(0);
}

void MainWidget::calAfcStatus(int cycles, bool done)
{
    qDebug("Cycles %d / %d, done: %d",
           cycles, calCyclesLe->text().toInt(), done);
    if (done) {
        timer->stop();
        qDebug() << "Calibration spectrums are ready...";
    }
}

void MainWidget::calAfcCalcAndSave()
{
    QByteArray data;
    data.append(calData[0]);
    data.append(calData[1]);
    if (data.size() == 65536) {
        qDebug() << "Calibration calculating is started...";
        calibrator->Calibrate(data, calCyclesLe->text().toInt());
        calibrator->SaveCalibration();
        qDebug() << "Calibration is saved";
    }
    else {
        qDebug() << "Error! Wrong Data Size: " << data.size();
    }
}

void MainWidget::calAfcGetData(QByteArray data)
{
    if (ms40Rb->isChecked())
        calData[1] = data;
    if (ms80Rb->isChecked())
        calData[0] = data;
}

void MainWidget::statusMessage(QString msg)
{
    statusLbl->setText(msg);
}

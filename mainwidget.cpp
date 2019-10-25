#include "mainwidget.h"
#include <QProcess>
#include <QGridLayout>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
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

    panel = new PanelConnect;
    panel->cmdGetVersion();

    calibrator = new Calibrator2;

    timer = new QTimer;

    startBtn = new QPushButton("Start MainMode");
    getLastLogBtn = new QPushButton("Read ErrCode");
    startCalBtn = new QPushButton("Start Calibr.");
    readCalBtn = new QPushButton("Read Calibr.");
    ms40Rb = new QRadioButton("40 ms");
    ms80Rb = new QRadioButton("80 ms");
    calCyclesLe = new QLineEdit("50");
    QGridLayout* gLayout = new QGridLayout;
    gLayout->addWidget(startBtn, 0, 0, 1, 2);
    gLayout->addWidget(getLastLogBtn, 1, 0, 1, 2);
    gLayout->addWidget(ms40Rb, 2, 0, 1, 1);
    gLayout->addWidget(ms80Rb, 2, 1, 1, 1);
    gLayout->addWidget(calCyclesLe, 3, 0, 1, 1);
    gLayout->addWidget(startCalBtn, 3, 1, 1, 1);
    gLayout->addWidget(readCalBtn, 4, 0, 1, 2);

    setLayout(gLayout);
//    setFixedSize(200, 100);

    connect(startBtn, SIGNAL(clicked()), this, SLOT(onStartBtn()));
    connect(getLastLogBtn, SIGNAL(clicked()), this, SLOT(onGetLastLogBtn()));
    connect(startCalBtn, SIGNAL(clicked()), this, SLOT(onStartCalBtn()));
    connect(readCalBtn, SIGNAL(clicked()), this, SLOT(onReadCalBtn()));
    connect(ms40Rb, SIGNAL(toggled(bool)), this, SLOT(onMs40Rb(bool)));
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(panel, SIGNAL(cmdCalAfcStatusReady(int, bool)),
            this, SLOT(calAfcStatus(int, bool)));
    connect(panel, SIGNAL(cmdCalAfcDataReady(QByteArray)),
            this, SLOT(calAfcCalcAndSave(QByteArray)));
}

MainWidget::~MainWidget()
{

}

void MainWidget::onStartBtn()
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

void MainWidget::calAfcCalcAndSave(QByteArray data)
{
    qDebug() << "Calibration calculating is started...";
    calibrator->Calibrate(data, calCyclesLe->text().toInt());
    calibrator->SaveCalibration();
    qDebug() << "Calibration is saved";
}

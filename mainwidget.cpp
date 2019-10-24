#include "mainwidget.h"
#include <QProcess>
#include <QGridLayout>

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

    startBtn = new QPushButton("Start");
    getLastLogBtn = new QPushButton("Read ErrCode");
    ms40Rb = new QRadioButton("40 ms");
    ms80Rb = new QRadioButton("80 ms");
    QGridLayout* gLayout = new QGridLayout;
    gLayout->addWidget(startBtn, 0, 0, 1, 2);
    gLayout->addWidget(getLastLogBtn, 1, 0, 1, 2);
    gLayout->addWidget(ms40Rb, 2, 0, 1, 1);
    gLayout->addWidget(ms80Rb, 2, 1, 1, 1);

    setLayout(gLayout);
//    setFixedSize(200, 100);

    connect(startBtn, SIGNAL(clicked()), this, SLOT(onStartBtn()));
    connect(getLastLogBtn, SIGNAL(clicked()), this, SLOT(onGetLastLogBtn()));
    connect(ms40Rb, SIGNAL(toggled(bool)), this, SLOT(onMs40Rb(bool)));

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

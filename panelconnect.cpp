#include "panelconnect.h"
#include <QDebug>

PanelConnect::PanelConnect(QObject *parent) : QObject(parent)
{
    udpConnect = new QUdpSocket(this);
    udpConnect->bind(UDP_PORT_TELEM_MODE_IN, QUdpSocket::ShareAddress);
    dissIPAddr = QHostAddress("192.168.1.163");
    connect(udpConnect, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
}

void PanelConnect::cmdGetVersion()
{
    QByteArray cmd_send_ba;
    cmd_send = PANEL_BUILD_MK;
    cmd_send_ba.append(QByteArray::fromRawData((char*)&cmd_send, 1));
    udpConnect->writeDatagram(cmd_send_ba, dissIPAddr, UDP_PORT_TELEM_MODE_OUT);
}

// 0xAC
void PanelConnect::cmdMainModeStart()
{
    QByteArray cmd_send_ba;
    cmd_send = PANEL_MAIN_MODE_SET;
    cmd_send_ba.append(QByteArray::fromRawData((char*)&cmd_send, 1));
    cmd_send_ba.append(QByteArray::fromHex("01"));
    udpConnect->writeDatagram(cmd_send_ba, dissIPAddr, UDP_PORT_TELEM_MODE_OUT);
}

// 0xB9
void PanelConnect::cmdMainModeSetParams()
{
    QByteArray cmd_send_ba;
    TechModeUnion mainModeParams;
    mainModeParams.f.missStartControl = 1;
    mainModeParams.f.missLog = 1;
    mainModeParams.f.banChange40_80 = 1;
    mainModeParams.f.banChangeIm = 1;
    mainModeParams.f.resetMK = 0;
    mainModeParams.f.banFpga40ms = 0;
    mainModeParams.f.banUseDAngles = 0;
    mainModeParams.f.startFpgaIn40 = 0;
    cmd_send = PANEL_TECH_MODE_SET;
    cmd_send_ba.append(QByteArray::fromRawData((char*)&cmd_send, 1));
    cmd_send_ba.append(QByteArray::fromRawData((char*)&mainModeParams.v, 1));
    udpConnect->writeDatagram(cmd_send_ba, dissIPAddr, UDP_PORT_TELEM_MODE_OUT);
}

// 0xB6
void PanelConnect::cmdMainModeSetFpga4080(bool ms40)
{
    QByteArray cmd_send_ba;
    cmd_send = PANEL_40_80_SET;
    cmd_send_ba.append(QByteArray::fromRawData((char*)&cmd_send, 1));
    if (ms40)
        cmd_send_ba.append(QByteArray::fromHex("00"));
    else
        cmd_send_ba.append(QByteArray::fromHex("01"));
    udpConnect->writeDatagram(cmd_send_ba, dissIPAddr, UDP_PORT_TELEM_MODE_OUT);
}

// 0xC9
void PanelConnect::cmdGetLastLog()
{
    QByteArray cmd_send_ba;
    cmd_send = PANEL_LAST_LOG_GET;
    cmd_send_ba.append(QByteArray::fromRawData((char*)&cmd_send, 1));
    udpConnect->writeDatagram(cmd_send_ba, dissIPAddr, UDP_PORT_TELEM_MODE_OUT);
}

void PanelConnect::processPendingDatagrams()
{
    QByteArray datagramRec;
    while (udpConnect->hasPendingDatagrams()) {
        datagramRec.resize(udpConnect->pendingDatagramSize());
        udpConnect->readDatagram(datagramRec.data(), datagramRec.size());
    }

    panelAnswerProcess(datagramRec);
}

void PanelConnect::panelAnswerProcess(QByteArray datagramRec)
{
    quint8* data_rec = (quint8*)datagramRec.data();
    cmd_rec = data_rec[0];
    cmd_rec_status = data_rec[1];
//    qDebug() << datagramRec.toHex();
    if (cmd_rec == cmd_send) {

        // Check error in answer
        if (cmd_rec_status == PANEL_DONE) {
            //            qDebug() << "Success";
            // Answer processing
            switch (cmd_rec) {
            case PANEL_BUILD_MK:
                qDebug() << "Version is read";
                break;
            case PANEL_TECH_MODE_SET:
                qDebug() << "Main mode params is setted...";
                cmdMainModeStart();
                break;
            case PANEL_MAIN_MODE_SET:
                qDebug() << "Main mode is started...";
                break;
            case PANEL_LAST_LOG_GET:
                EthLogErrorStructTypeDef* lastLog =
                        (EthLogErrorStructTypeDef*)&data_rec[2];
                qDebug("Last Error is: 0x%02X", lastLog->log.error);
                break;
            }
        }
        else if (cmd_rec_status == PANEL_ERROR)
            qDebug() << "Error";

    }
}

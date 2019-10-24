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
    QByteArray cmd;
    cmd.append(QByteArray::fromHex("A0"));
    udpConnect->writeDatagram(cmd, dissIPAddr, UDP_PORT_TELEM_MODE_OUT);
}

void PanelConnect::processPendingDatagrams()
{
    QByteArray datagramRec;
    while (udpConnect->hasPendingDatagrams()) {
        datagramRec.resize(udpConnect->pendingDatagramSize());
        udpConnect->readDatagram(datagramRec.data(), datagramRec.size());
    }
    qDebug() << datagramRec.toHex();
}

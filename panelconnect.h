#ifndef PANELCONNECT_H
#define PANELCONNECT_H

#include <QObject>
#include <QUdpSocket>

#include "disstypedef.h"

#define UDP_PORT_TELEM_MODE_IN  (45454)
#define UDP_PORT_TELEM_MODE_OUT (45454)

class PanelConnect : public QObject
{
    Q_OBJECT
public:
    explicit PanelConnect(QObject *parent = 0);
    void cmdGetVersion();           // 0xA0
    void cmdMainModeStart();        // 0xAC
    void cmdMainModeSetParams();    // 0xB9
    void cmdMainModeSetFpga4080(bool ms40);  // 0xB6
    void cmdGetLastLog();           // 0xC9
    void cmdStartCalAfc(int cycles);
    void cmdCalAfcGetStatus();
    void cmdCalAfcGetData(quint16 part_num);

private:
    QUdpSocket* udpConnect;
    QHostAddress dissIPAddr;
    quint8 cmd_send, cmd_rec, cmd_rec_status;
    quint16 part_num_send;
    QByteArray calData;

    void panelSendCmd(quint8 cmd, QByteArray data = QByteArray());
    void panelAnswerProcess(QByteArray datagramRec);

signals:
    void cmdGetVersionReady();
    void cmdCalAfcStatusReady(int, bool);
    void cmdCalAfcDataReady(QByteArray);

private slots:
    void processPendingDatagrams();

};

#endif // PANELCONNECT_H

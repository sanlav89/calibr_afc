#ifndef PANELCONNECT_H
#define PANELCONNECT_H

#include <QObject>
#include <QUdpSocket>

#include "disstypedef.h"

#define UDP_PORT_TELEM_MODE_IN  (45454)
#define UDP_PORT_TELEM_MODE_OUT (45454)

enum {
    ST_CONNECT_FAIL = 0,
    ST_CONNECT_READY,
    ST_READY_TO_SET_4080MS,
    ST_READY_TO_START_CALIBR,
    ST_ACCUM_CALIBR_PERFOMING,
    ST_READY_TO_READ_CALIBR_DATA,
    ST_READING_DATA_PERFOMING,
    ST_READING_DATA_ERROR,
    ST_READING_DATA_DONE
};

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
    void cmdCalAfcSetCtrlGetStatus(int cycles, bool status);
    void cmdCalAfcGetData(quint16 part_num);
    void cmdCalAfcGetDataRepeat();
    quint8 getStatus();

private:
    QUdpSocket* udpConnect;
    QHostAddress dissIPAddr;
    quint8 cmd_send, cmd_rec, cmd_rec_status;
    quint16 part_num_send;
    QByteArray calData;
    quint8 status;

    void panelSendCmd(quint8 cmd, QByteArray data = QByteArray());
    void panelAnswerProcess(QByteArray datagramRec);

signals:
    void panelStatus(quint8);
    void cmdCalAfcStatusReady(int, bool);
    void cmdCalAfcGetDataPartReady(quint16);
    void cmdCalAfcDataReady(QByteArray);


private slots:
    void processPendingDatagrams();

};

#endif // PANELCONNECT_H

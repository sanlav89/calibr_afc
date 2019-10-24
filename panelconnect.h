#ifndef PANELCONNECT_H
#define PANELCONNECT_H

#include <QObject>
#include <QUdpSocket>

#define UDP_PORT_TELEM_MODE_IN  (45454)
#define UDP_PORT_TELEM_MODE_OUT (45454)

class PanelConnect : public QObject
{
    Q_OBJECT
public:
    explicit PanelConnect(QObject *parent = 0);
    void cmdGetVersion();

private:
    QUdpSocket* udpConnect;
    QHostAddress dissIPAddr;

signals:
    void cmdGetVersionReady();

private slots:
    void processPendingDatagrams();

};

#endif // PANELCONNECT_H

#ifndef PANELCONNECT_H
#define PANELCONNECT_H

#include <QUdpSocket>

class PanelConnect : public QUdpSocket
{
public:
    PanelConnect(QUdpSocket *parent = 0);
    ~PanelConnect();
};

#endif // PANELCONNECT_H

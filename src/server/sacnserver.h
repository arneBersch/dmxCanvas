#ifndef SACNSERVER_H
#define SACNSERVER_H

#include <QtWidgets>
#include <QtNetwork>

class SacnServer : public QWidget {
    Q_OBJECT
public:
    SacnServer();
    uint8_t dmxData[512];
private slots:
    void processPendingDatagrams();
private:
    QUdpSocket *socket;
    QLabel *counterLabel;
    unsigned long receivedPackets = 0;
};

#endif // SACNSERVER_H

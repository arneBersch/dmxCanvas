#ifndef SACNDATAGRAM_H
#define SACNDATAGRAM_H

#include <QtWidgets>
#include <QtNetwork>

class SacnDatagram {
public:
    SacnDatagram(QNetworkDatagram datagram);
    bool isValid();
    QString getSource();
    int getPriority();
    int getUniverse();
    uint8_t getChannel(int channel);
private:
    QByteArray data;
    QHostAddress sourceAddress;
    bool checkFlagsAndLength(int index);
};

#endif // SACNDATAGRAM_H

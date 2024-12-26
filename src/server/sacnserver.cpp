#include "sacnserver.h"

SacnServer::SacnServer() {
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    counterLabel = new QLabel("No packets received.");
    layout->addWidget(counterLabel);

    socket = new QUdpSocket();
    socket->bind(QHostAddress::AnyIPv4, 5568);
    socket->joinMulticastGroup(QHostAddress("239.255.0.1"));
    connect(socket, &QUdpSocket::readyRead, this, &SacnServer::processPendingDatagrams);
}


void SacnServer::processPendingDatagrams() {
    while (socket->hasPendingDatagrams()) {
        QByteArray data = socket->receiveDatagram().data();
        if ((data.size() >= 125)
            && (data.size() <= 638)
            // ROOT LAYER
            // Preamble Size
            && (data[0] == (char)0x00)
            && (data[1] == (char)0x10)
            // Post-amble Size
            && (data[2] == (char)0x00)
            && (data[3] == (char)0x00)
            // ACN Packet Identifier
            && (data[4] == (char)0x41)
            && (data[5] == (char)0x53)
            && (data[6] == (char)0x43)
            && (data[7] == (char)0x2d)
            && (data[8] == (char)0x45)
            && (data[9] == (char)0x31)
            && (data[10] == (char)0x2e)
            && (data[11] == (char)0x31)
            && (data[12] == (char)0x37)
            && (data[13] == (char)0x00)
            && (data[14] == (char)0x00)
            && (data[15] == (char)0x00)
            // Vector
            && (data[18] == (char)0x00)
            && (data[19] == (char)0x00)
            && (data[20] == (char)0x00)
            && (data[21] == (char)0x04)
            // FRAMING LAYER
            // Vector
            && (data[40] == (char)0x00)
            && (data[41] == (char)0x00)
            && (data[42] == (char)0x00)
            && (data[43] == (char)0x02)
            // Universe
            //&& (data.mid(113, 2).toInt(nullptr, 16) == 1)
            // DMP LAYER
            // Vector
            && (data[117] == (char)0x02)
            // Address Type & Data Type
            && (data[118] == (char)0xa1)
            // First Property Address
            && (data[119] == (char)0x00)
            && (data[120] == (char)0x00)
            // Address Increment
            && (data[121] == (char)0x00)
            && (data[122] == (char)0x01)) {
            receivedPackets++;
            qDebug() << "Received E1.31 data packet " << receivedPackets;
            for (int channel = 0; channel < 511; channel++) {
                if (channel <= (data.length() - 127)) {
                    dmxData[channel] = data[126 + channel];
                } else {
                    dmxData[channel] = 0;
                }
            }
        } else {
            qDebug() << "Received invalid data.";
        }
    }
    counterLabel->setText("Received Packets: " + QString::number(receivedPackets));
}

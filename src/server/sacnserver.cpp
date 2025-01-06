/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "sacnserver.h"

SacnServer::SacnServer() {
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    counterLabel = new QLabel("No packets were received in sACN universe " + QString::number(universe) + ".");
    layout->addWidget(counterLabel);

    QPushButton *setUniverseButton = new QPushButton("Set sACN Universe");
    connect(setUniverseButton, &QPushButton::clicked, this, &SacnServer::setUniverse);
    layout->addWidget(setUniverseButton);

    socket = new QUdpSocket();
    socket->bind(QHostAddress::AnyIPv4, 5568);
    socket->joinMulticastGroup(universeToHostAddress(universe));
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
            && (((256 * (uint8_t)data[113]) + (uint8_t)data[114]) == universe)
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
            counterLabel->setText("Received Packets on Universe " + QString::number(universe) + ": " + QString::number(receivedPackets));
        } else {
            qDebug() << "Received invalid data.";
        }
    }
}

void SacnServer::setUniverse() {
    bool ok = true;
    int newUniverse = QInputDialog::getInt(this, "Set sACN Universe", "Please insert the wanted sACN Universe. The Universe has to be between 1 and 63999.", universe, 1, 63999, 1, &ok);
    if (!ok) {
        return;
    }
    socket->leaveMulticastGroup(universeToHostAddress(universe));
    universe = newUniverse;
    socket->joinMulticastGroup(universeToHostAddress(universe));
    qDebug() << "Set sACN Universe to " << universe << " and Multicast address to " << universeToHostAddress(universe).toString() << ".";
    receivedPackets = 0;
    counterLabel->setText("No packets were received in sACN universe " + QString::number(universe) + ".");
}

QHostAddress SacnServer::universeToHostAddress(int universeId) {
    QString address = "239.255.";
    address += QString::number(universeId / 256);
    address += ".";
    address += QString::number(universeId % 256);
    return QHostAddress(address);
}

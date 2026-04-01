/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "sacnserver.h"

SacnServer::SacnServer() {
    QGridLayout* layout = new QGridLayout();
    setLayout(layout);

    QLabel *universeLabel = new QLabel("sACN Universe");
    layout->addWidget(universeLabel, 0, 0);
    universeSpinBox = new QSpinBox();
    universeSpinBox->setRange(SACN_MIN_UNIVERSE, SACN_MAX_UNIVERSE);
    connect(universeSpinBox, &QSpinBox::valueChanged, this, &SacnServer::setUniverse);
    layout->addWidget(universeSpinBox, 0, 1);

    QLabel *receivedPacketsLabel = new QLabel("Priority");
    layout->addWidget(receivedPacketsLabel, 1, 0);
    priorityLabel = new QLabel();
    layout->addWidget(priorityLabel, 1, 1);

    QLabel *sourceNameLabel = new QLabel("Source Name");
    layout->addWidget(sourceNameLabel, 2, 0);
    sourceLabel = new QLabel();
    layout->addWidget(sourceLabel, 2, 1);

    layout->setRowStretch(3, 1);

    dataLossTimer = new QTimer();
    dataLossTimer->setSingleShot(true);
    connect(dataLossTimer, &QTimer::timeout, this, &SacnServer::dataLoss);

    dataLoss();
    reset();
}

void SacnServer::dataLoss() {
    priority = SACN_MIN_PRIORITY;

    priorityLabel->setText(QString::number(priority));
    sourceLabel->setText("<span style='background-color: red;'>Not connected</span>");
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
            && (((256 * (uint8_t)data[113]) + (uint8_t)data[114]) == universeSpinBox->value())
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
            && (data[122] == (char)0x01)
        ) {
            uint8_t dataPriority = data[108];

            if (dataPriority >= priority) {
                dmxData = data.sliced(126);
                priority = dataPriority;
                priorityLabel->setText(QString::number(priority));

                QByteArray sourceName = data.mid(44, 64);
                const int sourceNameLastIndex = sourceName.indexOf(0x00);
                if (sourceNameLastIndex >= 0) {
                    sourceName = sourceName.first(sourceNameLastIndex);
                }
                sourceLabel->setText(sourceName);

                dataLossTimer->start(SACN_NETWORK_DATA_LOSS_TIMEOUT);
            }
        } else {
            qDebug() << "Received invalid data.";
        }
    }
}

void SacnServer::setUniverse(int universe) {
    if (universe < SACN_MIN_UNIVERSE || universe > SACN_MAX_UNIVERSE) {
        return;
    }

    if (universeSpinBox->value() != universe) {
        universeSpinBox->setValue(universe);
    }

    const QHostAddress address = QHostAddress(SACN_ADDRESS_FORMAT.arg(universe / 256).arg(universe % 256));

    delete socket;
    socket = new QUdpSocket();
    socket->bind(QHostAddress::AnyIPv4, SACN_PORT);
    for (QNetworkInterface interface : QNetworkInterface::allInterfaces()) {
        socket->joinMulticastGroup(address, interface);
    }
    connect(socket, &QUdpSocket::readyRead, this, &SacnServer::processPendingDatagrams);

    qDebug() << "Set sACN Universe to " << universe << " and Multicast address to " << address.toString() << ".";
}

int SacnServer::getUniverse() {
    return universeSpinBox->value();
}

void SacnServer::reset() {
    setUniverse(SACN_MIN_UNIVERSE);
}

uint8_t SacnServer::getChannelValue(int channel) {
    if (channel < 1 || channel > dmxData.length()) {
        return 0;
    }

    return dmxData.at(channel - 1);
}

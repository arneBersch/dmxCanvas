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

    QLabel *statusLabel = new QLabel("Status");
    layout->addWidget(statusLabel, 1, 0);
    sourceLabel = new QLabel();
    layout->addWidget(sourceLabel, 1, 1);

    layout->setRowStretch(2, 1);

    dataLossTimer = new QTimer();
    dataLossTimer->setSingleShot(true);
    connect(dataLossTimer, &QTimer::timeout, this, &SacnServer::dataLoss);

    dataLoss();
    reset();
}

void SacnServer::dataLoss() {
    priority = SACN_MIN_PRIORITY;

    sourceLabel->setText("Not Connected");
    sourceLabel->setStyleSheet("* { background-color: red; }");
}

void SacnServer::processPendingDatagrams() {
    while (socket->hasPendingDatagrams()) {
        SacnDatagram datagram = SacnDatagram(socket->receiveDatagram());

        if (datagram.isValid() && (datagram.getUniverse() == universeSpinBox->value())) {
            if (datagram.getPriority() >= priority) {
                lastDatagram = datagram;
                priority = datagram.getPriority();
                sourceLabel->setText("Connected: " + datagram.getSource());
                sourceLabel->setStyleSheet("* { background-color: green; }");

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
    return lastDatagram.getChannel(channel);
}

float SacnServer::get8BitChannelRatio(int channel) {
    return (float)getChannelValue(channel) / 255;
}

float SacnServer::get16BitChannelRatio(int channel) {
    return (float)(getChannelValue(channel) * 256 + getChannelValue(channel + 1)) / 65535;
}

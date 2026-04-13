/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SACNSERVER_H
#define SACNSERVER_H

#include <QtWidgets>
#include <QtNetwork>

#include "sacndatagram.h"

class SacnServer : public QWidget {
    Q_OBJECT
public:
    SacnServer();
    void reset();
    uint8_t getChannelValue(int channel);
    void setUniverse(int newUniverse);
    int getUniverse();
private slots:
    void processPendingDatagrams();
    void dataLoss();
private:
    QUdpSocket *socket = new QUdpSocket();
    QLabel *sourceLabel;
    QSpinBox *universeSpinBox;
    QTimer* dataLossTimer;

    const int SACN_PORT = 5568;
    const int SACN_MIN_UNIVERSE = 1;
    const int SACN_MAX_UNIVERSE = 63999;
    const QString SACN_ADDRESS_FORMAT = "239.255.%1.%2";
    const int SACN_MIN_PRIORITY = 0;
    const int SACN_NETWORK_DATA_LOSS_TIMEOUT = 2500;

    int priority;
    SacnDatagram lastDatagram = SacnDatagram(QNetworkDatagram());
};

#endif // SACNSERVER_H

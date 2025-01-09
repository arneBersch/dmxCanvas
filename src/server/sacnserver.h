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

class SacnServer : public QWidget {
    Q_OBJECT
public:
    SacnServer();
    uint8_t dmxData[512] = {0};
    int universe = 1;
public slots:
    void setUniverse(int newUniverse);
private slots:
    void processPendingDatagrams();
private:
    const int SACN_PORT = 5568;
    const int SACN_MIN_UNIVERSE = 1;
    const int SACN_MAX_UNIVERSE = 63999;
    QUdpSocket *socket;
    QLabel *packetsCounterLabel;
    unsigned long receivedPackets = 0;
    QHostAddress universeToHostAddress(int universe);
};

#endif // SACNSERVER_H

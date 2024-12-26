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
    uint8_t dmxData[512];
private slots:
    void processPendingDatagrams();
private:
    QUdpSocket *socket;
    QLabel *counterLabel;
    unsigned long receivedPackets = 0;
};

#endif // SACNSERVER_H

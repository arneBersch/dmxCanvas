/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "canvaswindow.h"

CanvasWindow::CanvasWindow(QWidget *parent, bool fullscreen, ObjectList *objectList, SacnServer *sacnServer) : QWidget(parent, Qt::Window) {
    objects = objectList;
    sacn = sacnServer;
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("dmxCanvas");
    setStyleSheet("background-color: black;");
    if (fullscreen) {
        connect(new QShortcut(QKeySequence(Qt::Key_Escape), this), &QShortcut::activated, this, &CanvasWindow::close);
        setCursor(QCursor(Qt::BlankCursor));
        showFullScreen();
    } else {
        resize(720, 480);
        show();
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&CanvasWindow::update));
    timer->start(20); // update Display every 20ms (at 50Hz)
}

void CanvasWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setCompositionMode(QPainter::CompositionMode_Plus);
    for (int objectRow = 0; objectRow < objects->rowCount(); objectRow++) {
        int address = objects->data(objects->index(objectRow, ObjectListColumns::ChannelColumn), Qt::DisplayRole).toInt();
        int x = sacn->dmxData[address - 1] * width() / 255;;
        int y = sacn->dmxData[address] * height() / 255;
        int size = sacn->dmxData[address + 1] * height() / 255;
        int alpha = sacn->dmxData[address + 2];
        int red = 255 - sacn->dmxData[address + 3];
        int green = 255 - sacn->dmxData[address + 4];
        int blue = 255 - sacn->dmxData[address + 5];
        QBrush brush(Qt::SolidPattern);
        brush.setColor(QColor(red, green, blue, alpha));
        painter.setBrush(brush);
        painter.drawEllipse((x - (size / 2)), (y - (size / 2)), size, size);
    }
}

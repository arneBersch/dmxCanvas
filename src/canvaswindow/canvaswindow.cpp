/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "canvaswindow.h"

CanvasWindow::CanvasWindow(QWidget *parent, bool fullscreen, ObjectList *objectList, MediaSources *mediaSources, SacnServer *sacn) : QWidget(parent, Qt::Window) {
    objects = objectList;
    media = mediaSources;
    sacnServer = sacn;
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("dmxCanvas");
    setStyleSheet("background-color: black;");
    if (fullscreen) {
        new QShortcut(Qt::Key_Escape, this, [this]{ close(); });
        setCursor(QCursor(Qt::BlankCursor));
        showFullScreen();
    } else {
        resize(720, 480);
        show();
    }
    QImageReader::setAllocationLimit(0);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&CanvasWindow::update));
    timer->start(20); // update Display every 20ms (at 50Hz)
}

void CanvasWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setCompositionMode(QPainter::CompositionMode_Plus);
    objects->drawObjects(&painter, media, sacnServer);
}

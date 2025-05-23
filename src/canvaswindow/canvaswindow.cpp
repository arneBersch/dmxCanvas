/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "canvaswindow.h"

CanvasWindow::CanvasWindow(QWidget *parent, bool fullscreen, ObjectList *objectList, MediaSources *mediaSources, SacnServer *sacnServer) : QWidget(parent, Qt::Window) {
    objects = objectList;
    media = mediaSources;
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
        int address = objects->data(objects->index(objectRow, ObjectListColumns::AddressColumn), Qt::DisplayRole).toInt();
        QString objectType = objects->data(objects->index(objectRow, ObjectListColumns::TypeColumn), Qt::DisplayRole).toString();
        if (objectType.startsWith("Virtual Beam")) {
            int x = sacn->getChannelValue(address) * width() / 255;
            int y = sacn->getChannelValue(address + 1) * height() / 255;
            if (objectType.endsWith("(9 Channels)")) {
                x = (sacn->getChannelValue(address) * 256 + sacn->getChannelValue(address + 1)) * width() / 65535;
                y = (sacn->getChannelValue(address + 2) * 256 + sacn->getChannelValue(address + 3)) * height() / 65535;
                address += 2;
            }
            int size = sacn->getChannelValue(address + 2) * height() / 255;
            int alpha = sacn->getChannelValue(address + 3);
            int red = 255 - sacn->getChannelValue(address + 4);
            int green = 255 - sacn->getChannelValue(address + 5);
            int blue = 255 - sacn->getChannelValue(address + 6);
            QBrush brush(Qt::SolidPattern);
            brush.setColor(QColor(red, green, blue, alpha));
            painter.setBrush(brush);
            painter.drawEllipse((x - (size / 2)), (y - (size / 2)), size, size);
        } else if (objectType.startsWith("Image")) {
            int x = sacn->getChannelValue(address) * width() / 255;
            int y = sacn->getChannelValue(address + 1) * height() / 255;
            if (objectType.endsWith("(7 Channels)")) {
                x = (sacn->getChannelValue(address) * 256 + sacn->getChannelValue(address + 1)) * width() / 65535;
                y = (sacn->getChannelValue(address + 2) * 256 + sacn->getChannelValue(address + 3)) * height() / 65535;
                address += 2;
            }
            int size = sacn->getChannelValue(address + 2) * height() / 255;
            int brightness = sacn->getChannelValue(address + 3);
            int imageIndex = sacn->getChannelValue(address + 4);
            QString imagePath = QString();
            QDir directory = QDir(media->imageDirectory);
            if (directory.exists()) {
                QStringList images = directory.entryList(QDir::Files);
                foreach(QString fileName, images) {
                    bool isNumber = false;
                    int number = fileName.split(".")[0].toInt(&isNumber);
                    if (isNumber && (number == imageIndex)) {
                        imagePath = directory.absoluteFilePath(fileName);
                    }
                }
            }
            if (!imagePath.isEmpty()) {
                QImage image(imagePath);
                QImage alpha(image);
                QPainter alphaPainter(&alpha);
                alphaPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                alphaPainter.fillRect(alpha.rect(), QColor(brightness, brightness, brightness));
                alphaPainter.end();
                image.setAlphaChannel(alpha);
                if (!image.isNull()) {
                    int width = (image.width() * size / image.height());
                    painter.drawImage(QRect((x - (width / 2)), (y - (size / 2)), width, size), image);
                }
            }
        }
    }
}

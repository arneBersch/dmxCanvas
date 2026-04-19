/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "canvasobject.h"

CanvasObject::CanvasObject() {}

QString CanvasObject::getName() {
    return name;
}

void CanvasObject::setName(QString newName) {
    name = newName;
}

int CanvasObject::getAddress() {
    return address;
}

void CanvasObject::setAddress(int newAddress) {
    if (newAddress < MIN_ADDRESS || newAddress > MAX_ADDRESS) {
        return;
    }

    address = newAddress;
}

ObjectTypes::ObjectType CanvasObject::getType() {
    return type;
}

void CanvasObject::setType(ObjectTypes::ObjectType objectType) {
    type = objectType;
}

void CanvasObject::draw(QPainter* painter, MediaSources* media, SacnServer* sacnServer) {
    if (type == ObjectTypes::VirtualBeam8Bit || type == ObjectTypes::VirtualBeam16Bit) {
        float x = sacnServer->get8BitChannelRatio(address);
        float y = sacnServer->get8BitChannelRatio(address + 1);
        float size = sacnServer->get8BitChannelRatio(address + 2);
        float diffusion = sacnServer->get8BitChannelRatio(address + 3);
        int alpha = sacnServer->getChannelValue(address + 4);
        int cyan = sacnServer->getChannelValue(address + 5);
        int magenta = sacnServer->getChannelValue(address + 6);
        int yellow = sacnServer->getChannelValue(address + 7);
        if (type == ObjectTypes::VirtualBeam16Bit) {
            x = sacnServer->get16BitChannelRatio(address);
            y = sacnServer->get16BitChannelRatio(address + 2);
            size = sacnServer->get8BitChannelRatio(address + 4);
            diffusion = sacnServer->get8BitChannelRatio(address + 5);
            alpha = sacnServer->getChannelValue(address + 6);
            cyan = sacnServer->getChannelValue(address + 7);
            magenta = sacnServer->getChannelValue(address + 8);
            yellow = sacnServer->getChannelValue(address + 9);
        }
        x *= painter->window().width();
        y *= painter->window().height();
        size *= std::min(painter->window().width(), painter->window().height());

        if (alpha > 0) {
            QColor color = QColor(255 - cyan, 255 - magenta, 255 - yellow, alpha);
            if (diffusion > 0) {
                QRadialGradient gradient(x, y, size / 2);
                gradient.setColorAt(1 - diffusion, color);
                gradient.setColorAt(1, Qt::black);
                painter->setBrush(gradient);
            } else {
                painter->setBrush(color);
            }
            painter->drawEllipse((x - (size / 2)), (y - (size / 2)), size, size);
        }
    } else if (type == ObjectTypes::Image8Bit || type == ObjectTypes::Image16Bit) {
        float x = sacnServer->get8BitChannelRatio(address);
        float y = sacnServer->get8BitChannelRatio(address + 1);
        float size = sacnServer->get8BitChannelRatio(address + 2);
        int alpha = sacnServer->getChannelValue(address + 3);
        int imageIndex = sacnServer->getChannelValue(address + 4);
        if (type == ObjectTypes::Image16Bit) {
            x = sacnServer->get16BitChannelRatio(address);
            y = sacnServer->get16BitChannelRatio(address + 2);
            size = sacnServer->get8BitChannelRatio(address + 4);
            alpha = sacnServer->getChannelValue(address + 5);
            imageIndex = sacnServer->getChannelValue(address + 6);
        }
        x *= painter->window().width();
        y *= painter->window().height();
        size *= std::min(painter->window().width(), painter->window().height());

        if (alpha > 0) {
            QString imagePath = QString();
            QDir directory = QDir(media->getImageDirectory());
            if (directory.exists()) {
                QStringList images = directory.entryList(QDir::Files);
                for (QString fileName : images) {
                    bool isNumber = false;
                    int number = fileName.split(".")[0].toInt(&isNumber);
                    if (isNumber && (number == imageIndex)) {
                        imagePath = directory.absoluteFilePath(fileName);
                    }
                }
            }
            if (!imagePath.isEmpty()) {
                QImage image = QImage(imagePath);
                if (!image.isNull()) {
                    QImage alphaImage = QImage(image);
                    QPainter alphaPainter = QPainter(&alphaImage);
                    alphaPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                    alphaPainter.fillRect(alphaImage.rect(), QColor(alpha, alpha, alpha));
                    alphaPainter.end();
                    image.setAlphaChannel(alphaImage);
                    int width = (image.width() * size / image.height());
                    painter->drawImage(QRect((x - (width / 2)), (y - (size / 2)), width, size), image);
                }
            }
        }
    }
}

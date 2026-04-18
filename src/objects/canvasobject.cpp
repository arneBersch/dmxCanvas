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
        int x = sacnServer->getChannelValue(address) * painter->window().width() / 255;
        int y = sacnServer->getChannelValue(address + 1) * painter->window().height() / 255;
        if (type == ObjectTypes::VirtualBeam16Bit) {
            x = (sacnServer->getChannelValue(address) * 256 + sacnServer->getChannelValue(address + 1)) * painter->window().width() / 65535;
            y = (sacnServer->getChannelValue(address + 2) * 256 + sacnServer->getChannelValue(address + 3)) * painter->window().height() / 65535;
            address += 2;
        }
        int size = sacnServer->getChannelValue(address + 2) * painter->window().height() / 255;
        float diffusion = (float)sacnServer->getChannelValue(address + 3) / 255;
        int alpha = sacnServer->getChannelValue(address + 4);
        int red = 255 - sacnServer->getChannelValue(address + 5);
        int green = 255 - sacnServer->getChannelValue(address + 6);
        int blue = 255 - sacnServer->getChannelValue(address + 7);

        if (alpha > 0) {
            QColor color = QColor(red, green, blue, alpha);
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
        int x = sacnServer->getChannelValue(address) * painter->window().width() / 255;
        int y = sacnServer->getChannelValue(address + 1) * painter->window().height() / 255;
        int size = sacnServer->getChannelValue(address + 2) * painter->window().height() / 255;
        int alpha = sacnServer->getChannelValue(address + 3);
        int imageIndex = sacnServer->getChannelValue(address + 4);
        if (type == ObjectTypes::Image16Bit) {
            x = (sacnServer->getChannelValue(address) * 256 + sacnServer->getChannelValue(address + 1)) * painter->window().width() / 65535;
            y = (sacnServer->getChannelValue(address + 2) * 256 + sacnServer->getChannelValue(address + 3)) * painter->window().height() / 65535;
            size = sacnServer->getChannelValue(address + 4) * painter->window().height() / 255;
            alpha = sacnServer->getChannelValue(address + 5);
            imageIndex = sacnServer->getChannelValue(address + 6);
        }

        if (alpha > 0) {
            QString imagePath = QString();
            QDir directory = QDir(media->getImageDirectory());
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

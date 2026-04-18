/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "mediasources.h"

MediaSources::MediaSources(QWidget* parent) : QWidget(parent) {
    QGridLayout *layout = new QGridLayout();
    setLayout(layout);

    QLabel *imageLabel = new QLabel("Image Directory");
    layout->addWidget(imageLabel, 0, 0);

    imageSourceButton = new QPushButton();
    connect(imageSourceButton, &QPushButton::clicked, this, [this] { setImageDirectory(); });
    layout->addWidget(imageSourceButton, 0, 1);

    layout->setRowStretch(1, 1);
}

void MediaSources::resetSources() {
    setImageDirectory("");
}

QString MediaSources::getImageDirectory() {
    return imageDirectory;
}

void MediaSources::setImageDirectory(QString newDirectory) {
    if (newDirectory.isNull()) {
        newDirectory = QFileDialog::getExistingDirectory(this, "Selet Image Directory", imageDirectory);
        if (newDirectory.isEmpty()) {
            return;
        }
    }
    imageDirectory = newDirectory;
    imageSourceButton->setText(imageDirectory);
}

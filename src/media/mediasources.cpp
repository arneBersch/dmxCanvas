#include "mediasources.h"

MediaSources::MediaSources() {
    QGridLayout *layout = new QGridLayout();
    this->setLayout(layout);

    QLabel *imageLabel = new QLabel("Image Directory");
    layout->addWidget(imageLabel, 0, 0);

    imageSourceButton = new QPushButton();
    connect(imageSourceButton, &QPushButton::clicked, this, &MediaSources::setImageDirectory);
    layout->addWidget(imageSourceButton, 0, 1);
}

void MediaSources::setImageDirectory() {
    QString newDirectory = QFileDialog::getExistingDirectory(this, "Selet Image Directory", imageDirectory);
    if (newDirectory.isEmpty()) {
        return;
    }
    imageDirectory = newDirectory;
    imageSourceButton->setText(imageDirectory);
}

#include "mediasources.h"

MediaSources::MediaSources() {
    QGridLayout *layout = new QGridLayout();
    this->setLayout(layout);

    QLabel *imageLabel = new QLabel("Image Directory");
    layout->addWidget(imageLabel, 0, 0);

    imageSourceButton = new QPushButton();
    connect(imageSourceButton, &QPushButton::clicked, this, [this] { setImageDirectory(); });
    layout->addWidget(imageSourceButton, 0, 1);
}

void MediaSources::resetSources() {
    this->setImageDirectory("");
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

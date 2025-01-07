#include "mediasources.h"

MediaSources::MediaSources() {
    QGridLayout *layout = new QGridLayout();
    this->setLayout(layout);

    QLabel *imageLabel = new QLabel("Images");
    layout->addWidget(imageLabel, 0, 0);
}

#ifndef MEDIASOURCES_H
#define MEDIASOURCES_H

#include <QtWidgets>

class MediaSources : public QWidget {
    Q_OBJECT
public:
    MediaSources();
    QString imageDirectory;
private slots:
    void setImageDirectory();
private:
    QPushButton *imageSourceButton;
};

#endif // MEDIASOURCES_H

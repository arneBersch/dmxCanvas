#ifndef MEDIASOURCES_H
#define MEDIASOURCES_H

#include <QtWidgets>

class MediaSources : public QWidget {
    Q_OBJECT
public:
    MediaSources();
    void resetSources();
    QString getImageDirectory();
public slots:
    void setImageDirectory(QString directory = QString());
private:
    QString imageDirectory;
    QPushButton *imageSourceButton;
};

#endif // MEDIASOURCES_H

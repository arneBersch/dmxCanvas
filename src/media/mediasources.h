#ifndef MEDIASOURCES_H
#define MEDIASOURCES_H

#include <QtWidgets>

class MediaSources : public QWidget {
    Q_OBJECT
public:
    MediaSources();
    void resetSources();
    QString imageDirectory;
public slots:
    void setImageDirectory(QString directory = QString());
private:
    QPushButton *imageSourceButton;
};

#endif // MEDIASOURCES_H

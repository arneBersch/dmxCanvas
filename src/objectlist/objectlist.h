#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include <QtWidgets>

struct Object {
    QString name = QString();
    int channel = 1;
};

namespace ObjectListColumns {
enum {
    NameColumn,
    ChannelColumn,
};
}

class ObjectList : public QAbstractTableModel {
public:
    ObjectList(QString softwareVersion);
    void newFile();
    void openFile(QString filename);
    void saveFile(QString filename);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, const int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int columns, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
private:
    QList<Object*> objects;
    QString version;
};

#endif // OBJECTLIST_H

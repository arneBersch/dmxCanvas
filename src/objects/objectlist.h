/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include <QtWidgets>

#include "canvasobject.h"

namespace ObjectListColumns {
enum {
    NameColumn,
    AddressColumn,
    TypeColumn,
};
}

class ObjectList : public QAbstractTableModel {
public:
    ObjectList();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, const int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int columns, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
private:
    QList<CanvasObject*> objects = QList<CanvasObject*>();

    const QString VIRTUAL_BEAM_8BIT = "Virtual Beam (8 Channels)";
    const QString VIRTUAL_BEAM_16BIT = "Virtual Beam (10 Channels)";
    const QString IMAGE_8BIT = "Image (5 Channels)";
    const QString IMAGE_16BIT = "Image (7 Channels)";
};

#endif // OBJECTLIST_H

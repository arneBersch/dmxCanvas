/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "objectlist.h"

ObjectList::ObjectList() {}

int ObjectList::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return objects.length();
}

int ObjectList::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 3;
}

QVariant ObjectList::data(const QModelIndex &index, const int role) const {
    if (!index.isValid() || (role != Qt::DisplayRole)) {
        return QVariant();
    }
    const int row = index.row();
    const int column = index.column();
    if (row >= rowCount() || row < 0) {
        return QVariant();
    }
    Object* object = objects[row];
    if (column == ObjectListColumns::NameColumn) {
        return object->name;
    } else if (column == ObjectListColumns::AddressColumn) {
        return (QString::number(object->address));
    } else if (column == ObjectListColumns::TypeColumn) {
        if (object->type == ObjectTypes::VirtualBeam) {
            return "Virtual Beam";
        } else if (object->type == ObjectTypes::Image) {
            return "Image";
        }
    }
    return QVariant();
}

bool ObjectList::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        const int row = index.row();
        const int column = index.column();
        Object* object = objects[row];
        if (column == ObjectListColumns::NameColumn) {
            object->name = value.toString();
        } else if (column == ObjectListColumns::AddressColumn) {
            bool ok = false;
            int address = value.toInt(&ok);
            if (!ok || address < 1 || address > 506) {
                return false;
            }
            object->address = address;
        } else if (column == ObjectListColumns::TypeColumn) {
            if (value.toString() == "Virtual Beam") {
                object->type = ObjectTypes::VirtualBeam;
            } else if (value.toString() == "Image") {
                object->type = ObjectTypes::Image;
            }
        }
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
        return true;
    }
    return false;
}

QVariant ObjectList::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Orientation::Horizontal) {
        if (section == ObjectListColumns::NameColumn) {
            return "Name";
        } else if (section == ObjectListColumns::AddressColumn) {
            return "Address";
        } else if (section == ObjectListColumns::TypeColumn) {
            return "Type";
        }
        return QVariant();
    }
    return QVariant();
}

Qt::ItemFlags ObjectList::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ObjectList::insertRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, (position + rows - 1));
    for (int row = 0; row < rows; row++) {
        Object* object = new Object();
        objects.insert(position, object);
    }
    endInsertRows();
    return true;
}

bool ObjectList::removeRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, (position + rows - 1));
    for (int row = 0; row < rows; row++) {
        Object* object = objects[position];
        objects.removeAt(position);
        delete object;
    }
    endRemoveRows();
    return true;
}

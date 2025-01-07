/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "objectlist.h"

ObjectList::ObjectList(SacnServer *sacn, QString softwareVersion) {
    sacnServer = sacn;
    version = softwareVersion;
    objects = QList<Object*>();
}

void ObjectList::newFile() {
    removeRows(0, rowCount(), QModelIndex());
    sacnServer->setUniverse(1);
    qDebug() << "Opened new file.";
}

void ObjectList::openFile(QString filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox errorBox;
        errorBox.setText("Can't open file.");
        errorBox.exec();
        return;
    }
    QXmlStreamReader fileStream(&file);
    if ((fileStream.readNextStartElement()) && (fileStream.name().toString() == "Workspace")) {
        while (fileStream.readNextStartElement()) {
            if (fileStream.name().toString() == "Creator") {
                while (fileStream.readNextStartElement()) {
                    if ((fileStream.name().toString() == "Name") && (fileStream.readElementText() != "dmxCanvas")) {
                        QMessageBox errorBox;
                        errorBox.setText("This is not a dmxCanvas file.");
                        errorBox.exec();
                        return;
                    } else if ((fileStream.name().toString() == "Version") && (fileStream.readElementText() != version)) {
                        QMessageBox errorBox;
                        errorBox.setText("This dmxCanvas version isn't compatible to the current version (" + version + ").");
                        errorBox.exec();
                        return;
                    }
                }
            } else if (fileStream.name().toString() == "Input") {
                while (fileStream.readNextStartElement()) {
                    if (fileStream.name().toString() == "Universe") {
                        bool ok = true;
                        int universe = fileStream.readElementText().toInt(&ok);
                        if (!ok) {
                            QMessageBox errorBox;
                            errorBox.setText("Invalid Input Universe.");
                            errorBox.exec();
                            return;
                        }
                        sacnServer->setUniverse(universe);
                    }
                }
            } else if (fileStream.name().toString() == "Objects") {
                newFile();
                while (fileStream.readNextStartElement()) {
                    if (fileStream.name().toString() == "Object") {
                        insertRows(rowCount(), 1);
                        while (fileStream.readNextStartElement()) {
                            if (fileStream.name().toString() == "Name") {
                                setData(index((rowCount() - 1), ObjectListColumns::NameColumn), fileStream.readElementText());
                            } else if (fileStream.name().toString() == "Channel") {
                                setData(index((rowCount() - 1), ObjectListColumns::ChannelColumn), fileStream.readElementText());
                            }
                        }
                    }
                }
            }
        }
    }
    if (fileStream.hasError()) {
        QMessageBox errorBox;
        errorBox.setText("Can't open file because a XML passing error occured in line " + QString::number(fileStream.lineNumber()) + ": " + fileStream.errorString() + " (" + QString::number(fileStream.error()) + ")");
        errorBox.exec();
        return;
    }
    qDebug() << "Opened file " << filename;
}

void ObjectList::saveFile(QString filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox errorBox;
        errorBox.setText("Unable to save file.");
        errorBox.exec();
        return;
    }
    QXmlStreamWriter fileStream(&file);
    fileStream.setAutoFormatting(true);
    fileStream.writeStartDocument();
    fileStream.writeStartElement("Workspace");

    fileStream.writeStartElement("Creator");
    fileStream.writeTextElement("Name", "dmxCanvas");
    fileStream.writeTextElement("Version", version);
    fileStream.writeEndElement();

    fileStream.writeStartElement("Input");
    fileStream.writeTextElement("Universe", QString::number(sacnServer->universe));
    fileStream.writeEndElement();

    fileStream.writeStartElement("Objects");
    for (Object* object : objects) {
        fileStream.writeStartElement("Object");
        fileStream.writeTextElement("Name", object->name);
        fileStream.writeTextElement("Channel", QString::number(object->channel));
        fileStream.writeEndElement();
    }
    fileStream.writeEndElement();

    fileStream.writeEndElement();
    fileStream.writeEndDocument();
    qDebug() << "Saved file" << filename;
}

int ObjectList::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return objects.length();
}

int ObjectList::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
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
    } else if (column == ObjectListColumns::ChannelColumn) {
        return (QString::number(object->channel));
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
        } else if (column == ObjectListColumns::ChannelColumn) {
            bool ok = false;
            int channel = value.toString().toInt(&ok);
            if (!ok || channel < 1 || channel > 506) {
                return false;
            }
            object->channel = channel;
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
        } else if (section == ObjectListColumns::ChannelColumn) {
            return "Channel";
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

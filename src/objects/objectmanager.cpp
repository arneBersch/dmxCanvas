/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "objectmanager.h"

ObjectManager::ObjectManager(ObjectList* objects, QWidget* parent) : QWidget(parent) {
    objectList = objects;

    QVBoxLayout *objectsLayout = new QVBoxLayout();
    setLayout(objectsLayout);

    objectTable = new QTableView();
    objectTable->setModel(objectList);
    objectTable->horizontalHeader()->setStretchLastSection(true);
    objectTable->verticalHeader()->hide();
    objectTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //objectTable->setItemDelegateForColumn(ObjectListColumns::AddressColumn, new AddressItemDelegate(objectTable));
    //objectTable->setItemDelegateForColumn(ObjectListColumns::TypeColumn, new ObjectTypeItemDelegate(objectTable));
    objectsLayout->addWidget(objectTable);
    QPushButton *addObjectButton = new QPushButton("Add Object");
    connect(addObjectButton, &QPushButton::clicked, this, &ObjectManager::addObject);
    objectsLayout->addWidget(addObjectButton);
    QPushButton *removeObjectButton = new QPushButton("Remove Object");
    connect(removeObjectButton, &QPushButton::clicked, this, &ObjectManager::removeObject);
    objectsLayout->addWidget(removeObjectButton);
}

void ObjectManager::addObject() {
    objectList->insertRows(objectList->rowCount(), 1);
}

void ObjectManager::removeObject() {
    QModelIndexList selection = objectTable->selectionModel()->selectedRows();
    if (selection.size() <= 0) {
        return;
    }

    QMessageBox messageBox;
    messageBox.setText("Do you want to delete " + QString::number(selection.size()) + " Objects?");
    messageBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    if (messageBox.exec() != QMessageBox::Ok) {
        return;
    }

    std::sort(selection.begin(), selection.end(), [](QModelIndex a, QModelIndex b) { return a.row() > b.row(); });

    for (QModelIndex index : selection) {
        objectList->removeRows(index.row(), 1);
    }
}

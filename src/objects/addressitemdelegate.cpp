/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "addressitemdelegate.h"

AddressItemDelegate::AddressItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

QWidget* AddressItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(option);
    Q_UNUSED(index);
    QSpinBox *spinBox = new QSpinBox(parent);
    spinBox->setRange(CanvasObject::MIN_ADDRESS, CanvasObject::MAX_ADDRESS);
    return spinBox;
}

void AddressItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    const int address = index.data(Qt::DisplayRole).toInt();
    spinBox->setValue(address);
}

void AddressItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    model->setData(index, spinBox->value(), Qt::EditRole);
}

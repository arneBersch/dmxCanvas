#include "addressitemdelegate.h"

AddressItemDelegate::AddressItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

QWidget* AddressItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(option);
    Q_UNUSED(index);
    QSpinBox *spinBox = new QSpinBox(parent);
    spinBox->setRange(1, 512);
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

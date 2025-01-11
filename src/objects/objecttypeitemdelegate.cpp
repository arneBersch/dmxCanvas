#include "objecttypeitemdelegate.h"

ObjectTypeItemDelegate::ObjectTypeItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

QWidget* ObjectTypeItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->addItem("Virtual Beam");
    comboBox->addItem("Image");
    return comboBox;
}

void ObjectTypeItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    const int comboBoxIndex = comboBox->findText(index.data(Qt::DisplayRole).toString());
    comboBox->setCurrentIndex(comboBoxIndex);
}

void ObjectTypeItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

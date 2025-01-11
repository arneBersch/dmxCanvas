#ifndef ADDRESSITEMDELEGATE_H
#define ADDRESSITEMDELEGATE_H

#include <QtWidgets>

class AddressItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    AddressItemDelegate(QObject *parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // ADDRESSITEMDELEGATE_H

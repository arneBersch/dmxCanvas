#ifndef OBJECTTYPEITEMDELEGATE_H
#define OBJECTTYPEITEMDELEGATE_H

#include <QtWidgets>

class ObjectTypeItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    ObjectTypeItemDelegate(QObject *parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // OBJECTTYPEITEMDELEGATE_H

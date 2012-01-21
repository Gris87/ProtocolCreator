#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QComboBox>

class ListDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QString mLink;

    ListDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // LISTDELEGATE_H

#ifndef STRINGDELEGATE_H
#define STRINGDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QLineEdit>

class StringDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    StringDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // STRINGDELEGATE_H

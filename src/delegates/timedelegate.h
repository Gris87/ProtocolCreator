#ifndef TIMEDELEGATE_H
#define TIMEDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QTimeEdit>

class TimeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TimeDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TIMEDELEGATE_H

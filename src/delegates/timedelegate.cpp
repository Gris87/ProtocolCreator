#include "timedelegate.h"

TimeDelegate::TimeDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *TimeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTimeEdit *editor = new QTimeEdit(parent);

    return editor;
}

void TimeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ((QTimeEdit*)editor)->setTime(index.model()->data(index, Qt::EditRole).toTime());
}

void TimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *timeEdit = (QTimeEdit*)(editor);

    timeEdit->interpretText();

    model->setData(index, timeEdit->time(), Qt::EditRole);
}

void TimeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

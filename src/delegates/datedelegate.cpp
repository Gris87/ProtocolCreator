#include "datedelegate.h"

DateDelegate::DateDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *DateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDateEdit *editor = new QDateEdit(parent);

    return editor;
}

void DateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ((QDateEdit*)editor)->setDate(index.model()->data(index, Qt::EditRole).toDate());
}

void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit *dateEdit = (QDateEdit*)(editor);

    dateEdit->interpretText();

    model->setData(index, dateEdit->date(), Qt::EditRole);
}

void DateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

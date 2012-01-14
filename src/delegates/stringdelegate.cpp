#include "stringdelegate.h"

StringDelegate::StringDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *StringDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);

    return editor;
}

void StringDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ((QLineEdit*)editor)->setText(index.model()->data(index, Qt::EditRole).toString());
}

void StringDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = (QLineEdit*)(editor);

    model->setData(index, lineEdit->text(), Qt::EditRole);
}

void StringDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

#include "doubledelegate.h"

DoubleDelegate::DoubleDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *DoubleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMinimum(-2147483647);
    editor->setMaximum(2147483647);

    return editor;
}

void DoubleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ((QDoubleSpinBox*)editor)->setValue(index.model()->data(index, Qt::EditRole).toDouble());
}

void DoubleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = (QDoubleSpinBox*)(editor);

    spinBox->interpretText();

    model->setData(index, spinBox->value(), Qt::EditRole);
}

void DoubleDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

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
    editor->setDecimals(mDecimals);
    editor->setPrefix(mPrefix);
    editor->setSuffix(mPostfix);

    return editor;
}

void DoubleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString aData=index.model()->data(index, Qt::EditRole).toString();

    aData=aData.mid(mPrefix.length());
    aData.remove(aData.length()-mPostfix.length(), mPostfix.length());

    ((QDoubleSpinBox*)editor)->setValue(aData.toDouble());
}

void DoubleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = (QDoubleSpinBox*)(editor);

    spinBox->interpretText();

    model->setData(index, mPrefix+QString::number(spinBox->value(), 'f', mDecimals)+mPostfix, Qt::EditRole);
}

void DoubleDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

#include "listdelegate.h"

ListDelegate::ListDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *ListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);

    return editor;
}

void ListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = (QComboBox*)(editor);

    QString aValue=index.model()->data(index, Qt::EditRole).toString();

    int itemIndex=comboBox->findText(aValue);

    if (itemIndex>=0)
    {
        comboBox->setCurrentIndex(itemIndex);
    }
    else
    {
        comboBox->setEditText(aValue);
    }
}

void ListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = (QComboBox*)(editor);

    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void ListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

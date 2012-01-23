#include "src/other/global.h"

DataTable::DataTable(QWidget *parent) :
    QTableWidget(parent)
{
}

QModelIndex DataTable::indexFromItem(QTableWidgetItem *item) const
{
    return QTableWidget::indexFromItem(item);
}

void DataTable::commitData(QWidget *editor)
{
    VariableExtendedListFrame *aTable=(VariableExtendedListFrame*)mTable;

    if (
        itemDelegateForRow(currentRow())
        ||
        (
         aTable->typeColumns.at(currentColumn()).column->type()!=ctBool
         &&
         aTable->typeColumns.at(currentColumn()).column->type()!=ctExpression
         &&
         (
          aTable->typeColumns.at(currentColumn()).column->type()!=ctInteger
          ||
          !((IntegerColumn*)(aTable->typeColumns.at(currentColumn()).column))->mIsAutoInc
         )
        )
       )
    {
        QTableWidget::commitData(editor);
    }
}

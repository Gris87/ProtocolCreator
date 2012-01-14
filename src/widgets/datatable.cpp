#include "src/other/global.h"

DataTable::DataTable(QWidget *parent) :
    QTableWidget(parent)
{
}

void DataTable::commitData(QWidget *editor)
{
    VariableExtendedListFrame *aTable=(VariableExtendedListFrame*)mTable;

    if (
        columnSpan(currentRow(), currentColumn())>1
        ||
        (
         aTable->typeColumns.at(currentColumn()).column->type()!=ctBool
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

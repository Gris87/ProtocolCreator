#include "word.h"

WordTable::WordTable(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

WordTableCell* WordTable::cell(int row, int column)
{
    row++;
    column++;

    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Cell(int, int)", row, column);

        if (aObject==0)
        {
            qWarning()<<"WordTable::cell(): Retry #"+QString::number(retry+1)+" Couldn't get cell ("+QString::number(row-1)+", "+QString::number(column-1)+")";
            continue;
        }

        return new WordTableCell(aObject, this);
    }

    throw "WordTable::cell(): Couldn't get cell ("+QString::number(row-1)+", "+QString::number(column-1)+")";
}

#include "word.h"

WordTable::WordTable(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

WordTableRows* WordTable::rows()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Rows()");

        if (aObject==0)
        {
            qWarning()<<"WordTable::rows(): Retry #"+QString::number(retry+1)+" Couldn't get rows";
            continue;
        }

        return new WordTableRows(aObject, this);
    }

    throw "WordTable::rows(): Couldn't get rows";
}

WordTableColumns* WordTable::columns()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Columns()");

        if (aObject==0)
        {
            qWarning()<<"WordTable::columns(): Retry #"+QString::number(retry+1)+" Couldn't get columns";
            continue;
        }

        return new WordTableColumns(aObject, this);
    }

    throw "WordTable::columns(): Couldn't get columns";
}

WordTableCell* WordTable::cell(int row, int column)
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Cell(int, int)", row+1, column+1);

        if (aObject==0)
        {
            qWarning()<<"WordTable::cell(): Retry #"+QString::number(retry+1)+" Couldn't get cell ("+QString::number(row)+", "+QString::number(column)+")";
            continue;
        }

        return new WordTableCell(aObject, this);
    }

    throw "WordTable::cell(): Couldn't get cell ("+QString::number(row)+", "+QString::number(column)+")";
}

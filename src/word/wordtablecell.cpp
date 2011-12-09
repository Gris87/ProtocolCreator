#include "word.h"

WordTableCell::WordTableCell(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

WordRange* WordTableCell::range()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Range()");

        if (aObject==0)
        {
            qWarning()<<"WordTableCell::add(): Retry #"+QString::number(retry+1)+" Couldn't get range";
            continue;
        }

        return new WordRange(aObject, this);
    }

    throw "WordTableCell::add(): Couldn't get range";
}

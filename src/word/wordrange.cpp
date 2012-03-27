#include "word.h"

WordRange::WordRange(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

WordTables* WordRange::tables()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Tables()");

        if (aObject==0)
        {
            qWarning()<<"WordRange::tables(): Retry #"+QString::number(retry+1)+" Couldn't get tables";
            continue;
        }

        return new WordTables(aObject, this);
    }

    throw "WordRange::tables(): Couldn't get tables";
}

void WordRange::InsertAfter(QString aText)
{
    try
    {
        base_ax_object->dynamicCall("InsertAfter(QString)", aText);
    }
    catch (...)
    {
        qWarning()<<"WordRange::InsertAfter(): Couldn't insert text";
    }
}

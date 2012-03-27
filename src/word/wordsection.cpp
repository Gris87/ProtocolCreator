#include "word.h"

WordSection::WordSection(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

WordRange* WordSection::range()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Range()");

        if (aObject==0)
        {
            qWarning()<<"WordSection::range(): Retry #"+QString::number(retry+1)+" Couldn't get range";
            continue;
        }

        return new WordRange(aObject, this);
    }

    throw "WordSection::range(): Couldn't get range";
}

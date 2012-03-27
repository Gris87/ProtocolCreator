#include "word.h"

WordSections::WordSections(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

WordSection* WordSections::add()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Add()");

        if (aObject==0)
        {
            qWarning()<<"WordSections::add(): Retry #"+QString::number(retry+1)+" Couldn't add section";
            continue;
        }

        return new WordSection(aObject, this);
    }

    throw "WordSections::add(): Couldn't add section";
}

WordSection* WordSections::item(int index)
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Item(int)", index+1);

        if (aObject==0)
        {
            qWarning()<<"WordSections::item(): Retry #"+QString::number(retry+1)+" Couldn't get section "+QString::number(index);
            continue;
        }

        return new WordSection(aObject, this);
    }

    throw "WordSections::item(): Couldn't get section";
}

int WordSections::count()
{
    try
    {
        return base_ax_object->dynamicCall("Count()").toInt();
    }
    catch (...)
    {
        qWarning()<<"WordSections::count(): Couldn't receive count";
    }

    return 0;
}

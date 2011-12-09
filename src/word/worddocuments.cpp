#include "word.h"

WordDocuments::WordDocuments(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

WordDocument* WordDocuments::add()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Add()");

        if (aObject==0)
        {
            qWarning()<<"WordDocuments::add(): Retry #"+QString::number(retry+1)+" Couldn't add document";
            continue;
        }

        return new WordDocument(aObject, this);
    }

    throw "WordDocuments::add(): Couldn't add document";
}

WordDocument* WordDocuments::open(QString aPath)
{
    aPath=QDir::toNativeSeparators(aPath);

    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Open(const QString)", aPath);

        if (aObject==0)
        {
            qWarning()<<"WordDocuments::open(): Retry #"+QString::number(retry+1)+" Couldn't open document: "+aPath;
            continue;
        }

        return new WordDocument(aObject, this);
    }

    throw "WordDocuments::open(): Couldn't open document: "+aPath;
}

int WordDocuments::count()
{
    try
    {
        return base_ax_object->dynamicCall("Count()").toInt();
    }
    catch (...)
    {
        qWarning()<<"WordDocuments::count(): Couldn't receive count";
    }

    return 0;
}

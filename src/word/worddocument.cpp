#include "word.h"

WordDocument::WordDocument(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

WordSections* WordDocument::sections()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Sections()");

        if (aObject==0)
        {
            qWarning()<<"WordDocument::sections(): Retry #"+QString::number(retry+1)+" Couldn't get sections";
            continue;
        }

        return new WordSections(aObject, this);
    }

    throw "WordDocument::sections(): Couldn't get sections";
}

WordTables* WordDocument::tables()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Tables()");

        if (aObject==0)
        {
            qWarning()<<"WordDocument::tables(): Retry #"+QString::number(retry+1)+" Couldn't get tables";
            continue;
        }

        return new WordTables(aObject, this);
    }

    throw "WordDocument::tables(): Couldn't get tables";
}

WordRange* WordDocument::range()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Range()");

        if (aObject==0)
        {
            qWarning()<<"WordDocument::range(): Retry #"+QString::number(retry+1)+" Couldn't get range";
            continue;
        }

        return new WordRange(aObject, this);
    }

    throw "WordDocument::range(): Couldn't get range";
}

int WordDocument::fileFormat()
{
    try
    {
        return base_ax_object->dynamicCall("SaveFormat()").toInt();
    }
    catch (...)
    {
        qWarning()<<"WordDocument::fileFormat(): Couldn't receive fileFormat";
    }

    return 0;
}

void WordDocument::save()
{
    try
    {
        base_ax_object->dynamicCall("Save()");
    }
    catch (...)
    {
        qWarning()<<"WordDocument::save(): Couldn't save document";
    }
}

void WordDocument::saveAs(QString aFileName, int aFileFormat)
{
    try
    {
        if (QFile::exists(aFileName))
        {
            QFile::remove(aFileName);
        }

        base_ax_object->dynamicCall("SaveAs(const QString, conts int)", QDir::toNativeSeparators(aFileName), aFileFormat);
    }
    catch (...)
    {
        qWarning()<<"WordDocument::saveAs(): Couldn't save document";
    }
}

void WordDocument::saveAs(QString aFileName)
{
    try
    {
        if (QFile::exists(aFileName))
        {
            QFile::remove(aFileName);
        }

        base_ax_object->dynamicCall("SaveAs(const QString)", QDir::toNativeSeparators(aFileName));
    }
    catch (...)
    {
        qWarning()<<"WordDocument::SaveAs(): Couldn't save document";
    }
}

void WordDocument::saveAs2003(QString aFileName, int aDefaultFileFormat)
{
    /* Need to check formats
    if (aDefaultFileFormat==51)
    {
        aDefaultFileFormat=56;
    }
    */

    try
    {
        if (QFile::exists(aFileName))
        {
            QFile::remove(aFileName);
        }

        base_ax_object->dynamicCall("SaveAs(const QString, conts int)", QDir::toNativeSeparators(aFileName), aDefaultFileFormat);
    }
    catch (...)
    {
        qWarning()<<"WordDocument::saveAs2003(): Couldn't save workbook";
    }
}

void WordDocument::close()
{
    try
    {
        base_ax_object->dynamicCall("Close()");
    }
    catch (...)
    {
        qWarning()<<"WordDocument::close(): Couldn't close";
    }
}

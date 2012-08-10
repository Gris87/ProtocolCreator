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

int WordRange::computeStatistics(WdStatistic Statistic)
{
    try
    {
        return base_ax_object->dynamicCall("ComputeStatistics(WdStatistic)", Statistic).toInt();
    }
    catch (...)
    {
        qWarning()<<"WordRange::ComputeStatistics(): Couldn't compute statistics for"<<Statistic;
    }

    return 0;
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

QString WordRange::text()
{
    try
    {
        return base_ax_object->dynamicCall("Text()").toString();
    }
    catch (...)
    {
        qWarning()<<"WordRange::text(): Couldn't receive text";
    }

    return "";
}

#include "word.h"

WordTables::WordTables(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

WordTable* WordTables::add(QVariant aRange, int NumRows, int NumColumns, bool DefaultTableBehavior, bool AutoFitBehavior)
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Add(IDispatch, int, int, bool, bool)", aRange, NumRows, NumColumns, DefaultTableBehavior, AutoFitBehavior);

        if (aObject==0)
        {
            qWarning()<<"WordTables::add(): Retry #"+QString::number(retry+1)+" Couldn't add table";
            continue;
        }

        return new WordTable(aObject, this);
    }

    throw "WordTables::add(): Couldn't add table";
}

WordTable* WordTables::addOld(QVariant aRange, int NumRows, int NumColumns)
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("AddOld(IDispatch, int, int)", aRange, NumRows, NumColumns);

        if (aObject==0)
        {
            qWarning()<<"WordTables::add(): Retry #"+QString::number(retry+1)+" Couldn't add table";
            continue;
        }

        return new WordTable(aObject, this);
    }

    throw "WordTables::add(): Couldn't add table";
}

WordTable* WordTables::item(int index)
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Item(int)", index);

        if (aObject==0)
        {
            qWarning()<<"WordTables::add(): Retry #"+QString::number(retry+1)+" Couldn't get table #"+QString::number(index);
            continue;
        }

        return new WordTable(aObject, this);
    }

    throw "WordTables::add(): Couldn't get table #"+QString::number(index);
}

int WordTables::count()
{
    try
    {
        return base_ax_object->dynamicCall("Count()").toInt();
    }
    catch (...)
    {
        qWarning()<<"WordTables::count(): Couldn't receive count";
    }

    return 0;
}

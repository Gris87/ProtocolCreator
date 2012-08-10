#include "word.h"

WordTableRows::WordTableRows(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

int WordTableRows::count()
{
    try
    {
        return base_ax_object->dynamicCall("Count()").toInt();
    }
    catch (...)
    {
        qWarning()<<"WordTableRows::count(): Couldn't receive count";
    }

    return 0;
}

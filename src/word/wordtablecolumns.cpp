#include "word.h"

WordTableColumns::WordTableColumns(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

int WordTableColumns::count()
{
    try
    {
        return base_ax_object->dynamicCall("Count()").toInt();
    }
    catch (...)
    {
        qWarning()<<"WordTableColumns::count(): Couldn't receive count";
    }

    return 0;
}

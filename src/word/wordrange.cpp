#include "word.h"

WordRange::WordRange(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
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

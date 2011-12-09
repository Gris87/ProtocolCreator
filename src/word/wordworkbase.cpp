#include "word.h"

WordWorkBase::WordWorkBase(QAxObject *aBaseObject, QObject *parent) :
    WordBase(parent)
{
    base_ax_object=aBaseObject;
}

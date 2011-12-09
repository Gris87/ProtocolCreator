#include "word.h"

WordOptions::WordOptions(QAxObject *aBaseObject, QObject *parent) :
    WordWorkBase(aBaseObject, parent)
{
}

void WordOptions::SetCheckSpellingAsYouType(bool CheckSpellingAsYouType)
{
    try
    {
        base_ax_object->dynamicCall("SetCheckSpellingAsYouType(bool)", CheckSpellingAsYouType);
    }
    catch (...)
    {
        qWarning()<<"WordDocument::save(): Couldn't save workbook";
    }
}

bool WordOptions::CheckSpellingAsYouType()
{
    try
    {
        return base_ax_object->dynamicCall("CheckSpellingAsYouType()").toBool();
    }
    catch (...)
    {
        qWarning()<<"WordOptions::CheckSpellingAsYouType(): Couldn't receive CheckSpellingAsYouType";
    }

    return false;
}

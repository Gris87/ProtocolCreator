#include "src/other/global.h"

ExtendedListColumn::ExtendedListColumn() : ColumnType()
{
    init();
}

void ExtendedListColumn::init()
{
    mDefaultValue="";
    mLinkComponent="";
}

void ExtendedListColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColExtList");

    if (mDefaultValue!="")
    {
        aStream << QString("Default");
        aStream << mDefaultValue;
    }

    if (mLinkComponent!="")
    {
        aStream << QString("Link");
        aStream << mLinkComponent;
    }

    aStream << QString("ColEnd");
}

void ExtendedListColumn::loadFromStream(QDataStream &aStream)
{
    init();

    QString aMagicWord;

    while (!aStream.atEnd())
    {
        aStream >> aMagicWord;

        if (aMagicWord=="Default")
        {
            aStream >> mDefaultValue;
        }
        else
        if (aMagicWord=="Link")
        {
            aStream >> mLinkComponent;
        }
        else
        if (aMagicWord=="ColEnd")
        {
            break;
        }
    }
}

QString ExtendedListColumn::typeDescription()
{
    return "Расш. список (\""+mDefaultValue+"\"; \""+mLinkComponent+"\")";
}

EColumnType ExtendedListColumn::type()
{
    return ctExtendedList;
}

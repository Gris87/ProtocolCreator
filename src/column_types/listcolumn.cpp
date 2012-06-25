#include "src/other/global.h"

ListColumn::ListColumn() : ColumnType()
{
    init();
}

void ListColumn::init()
{
    mDefaultValue="";
    mLinkComponent="";
}

void ListColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColList");

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

void ListColumn::loadFromStream(QDataStream &aStream)
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

QString ListColumn::typeDescription()
{
    return "Список (\""+mDefaultValue+"\"; \""+mLinkComponent+"\")";
}

EColumnType ListColumn::type()
{
    return ctList;
}

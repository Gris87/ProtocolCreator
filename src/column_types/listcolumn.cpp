#include "src/other/global.h"

ListColumn::ListColumn() : ColumnType()
{
}

void ListColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColList");

    aStream << QString("Default");
    aStream << mDefaultValue;

    aStream << QString("ColEnd");
}

void ListColumn::loadFromStream(QDataStream &aStream)
{
    QString aMagicWord;

    while (!aStream.atEnd())
    {
        aStream >> aMagicWord;

        if (aMagicWord=="Default")
        {
            aStream >> mDefaultValue;
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
    return "Список (\""+mDefaultValue+"\")";
}

EColumnType ListColumn::type()
{
    return ctList;
}

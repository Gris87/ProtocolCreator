#include "src/other/global.h"

DateColumn::DateColumn() : ColumnType()
{
}

void DateColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColDate");

    aStream << QString("Default");
    aStream << mDefaultValue;

    aStream << QString("ColEnd");
}

void DateColumn::loadFromStream(QDataStream &aStream)
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

QString DateColumn::typeDescription()
{
    return "Дата ("+mDefaultValue.toString("dd.MM.yyyy")+")";
}

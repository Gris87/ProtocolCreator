#include "src/other/global.h"

DateColumn::DateColumn() : ColumnType()
{
    init();
}

void DateColumn::init()
{
    mDefaultValue=QDate(2000, 1, 1);
}

void DateColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColDate");

    if (mDefaultValue!=QDate(2000, 1, 1))
    {
        aStream << QString("Default");
        aStream << mDefaultValue;
    }

    aStream << QString("ColEnd");
}

void DateColumn::loadFromStream(QDataStream &aStream)
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

EColumnType DateColumn::type()
{
    return ctDate;
}

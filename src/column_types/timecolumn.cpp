#include "src/other/global.h"

TimeColumn::TimeColumn() : ColumnType()
{
    init();
}

void TimeColumn::init()
{
    mDefaultValue=QTime(0, 0);
}

void TimeColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColTime");

    if (mDefaultValue!=QTime(0, 0))
    {
        aStream << QString("Default");
        aStream << mDefaultValue;
    }

    aStream << QString("ColEnd");
}

void TimeColumn::loadFromStream(QDataStream &aStream)
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

QString TimeColumn::typeDescription()
{
    return "Время ("+mDefaultValue.toString("hh:mm:ss")+")";
}

EColumnType TimeColumn::type()
{
    return ctTime;
}

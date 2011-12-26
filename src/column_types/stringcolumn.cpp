#include "src/other/global.h"

StringColumn::StringColumn() : ColumnType()
{
}

void StringColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColString");

    aStream << QString("Default");
    aStream << mDefaultValue;

    aStream << QString("ColEnd");
}

void StringColumn::loadFromStream(QDataStream &aStream)
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

QString StringColumn::typeDescription()
{
    return "Строка (\""+mDefaultValue+"\")";
}

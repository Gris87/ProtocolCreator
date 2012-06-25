#include "src/other/global.h"

StringColumn::StringColumn() : ColumnType()
{
    init();
}

void StringColumn::init()
{
    mDefaultValue="";
}

void StringColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColString");

    if (mDefaultValue!="")
    {
        aStream << QString("Default");
        aStream << mDefaultValue;
    }

    aStream << QString("ColEnd");
}

void StringColumn::loadFromStream(QDataStream &aStream)
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

QString StringColumn::typeDescription()
{
    return "Строка (\""+mDefaultValue+"\")";
}

EColumnType StringColumn::type()
{
    return ctString;
}

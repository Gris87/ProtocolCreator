#include "src/other/global.h"

IntegerColumn::IntegerColumn() : ColumnType()
{
}

void IntegerColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColInteger");

    aStream << QString("Default");
    aStream << mDefaultValue;

    aStream << QString("AutoInc");
    aStream << mIsAutoInc;

    aStream << QString("Prefix");
    aStream << mPrefix;

    aStream << QString("Postfix");
    aStream << mPostfix;

    aStream << QString("ColEnd");
}

void IntegerColumn::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="AutoInc")
        {
            aStream >> mIsAutoInc;
        }
        else
        if (aMagicWord=="Prefix")
        {
            aStream >> mPrefix;
        }
        else
        if (aMagicWord=="Postfix")
        {
            aStream >> mPostfix;
        }
        else
        if (aMagicWord=="ColEnd")
        {
            break;
        }
    }
}

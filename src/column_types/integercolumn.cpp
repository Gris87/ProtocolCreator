#include "src/other/global.h"

IntegerColumn::IntegerColumn() : ColumnType()
{
}

void IntegerColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColInteger");

    aStream << QString("Default");
    aStream << mDefaultValue;

    aStream << QString("Decimals");
    aStream << mDecimals;

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
        if (aMagicWord=="Decimals")
        {
            aStream >> mDecimals;
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

QString IntegerColumn::typeDescription()
{
    QString aTypeDescription;

    aTypeDescription="Число ("+QString::number(mDefaultValue)+"; "+
                               QString::number(mDecimals)+"; ";

    if (mIsAutoInc)
    {
        aTypeDescription.append("X");
    }
    else
    {
        aTypeDescription.append("_");
    }

    aTypeDescription.append(
                            "; \""+mPrefix+"\"; "+
                            "\""+mPostfix+"\")"
                           );

    return aTypeDescription;
}

EColumnType IntegerColumn::type()
{
    return ctInteger;
}


#include "src/other/global.h"

IntegerColumn::IntegerColumn() : ColumnType()
{
    init();
}

void IntegerColumn::init()
{
    mDefaultValue=0;
    mDecimals=2;
    mIsAutoInc=false;
    mSplitRows=false;
    mPrefix="";
    mPostfix="";
}

void IntegerColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColInteger");

    if (mIsAutoInc)
    {
        aStream << QString("AutoInc");
        aStream << mIsAutoInc;
    }
    else
    {
        if (mSplitRows)
        {
            aStream << QString("SplitRows");
            aStream << mSplitRows;
        }
        else
        {
            if (mDecimals!=2)
            {
                aStream << QString("Decimals");
                aStream << mDecimals;
            }
        }

        if (mDefaultValue!=0)
        {
            aStream << QString("Default");
            aStream << mDefaultValue;
        }

        if (mPrefix!="")
        {
            aStream << QString("Prefix");
            aStream << mPrefix;
        }

        if (mPostfix!="")
        {
            aStream << QString("Postfix");
            aStream << mPostfix;
        }
    }

    aStream << QString("ColEnd");
}

void IntegerColumn::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="SplitRows")
        {
            aStream >> mSplitRows;
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

    aTypeDescription="Число (";

    if (mIsAutoInc)
    {
        aTypeDescription.append("A");
    }
    else
    if (mSplitRows)
    {
        aTypeDescription.append("S");
    }
    else
    {
        aTypeDescription.append("_");
    }

    aTypeDescription.append(
                            "; "+
                            QString::number(mDefaultValue)+"; "+
                            QString::number(mDecimals)+"; "+
                            "\""+mPrefix+"\"; "+
                            "\""+mPostfix+"\")"
                           );

    return aTypeDescription;
}

EColumnType IntegerColumn::type()
{
    return ctInteger;
}


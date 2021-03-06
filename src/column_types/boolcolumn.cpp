#include "src/other/global.h"

BoolColumn::BoolColumn() : ColumnType()
{
    init();
}

void BoolColumn::init()
{
    mDefaultValue=false;
}

void BoolColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColBool");

    if (mDefaultValue)
    {
        aStream << QString("Default");
        aStream << mDefaultValue;
    }

    aStream << QString("ColEnd");
}

void BoolColumn::loadFromStream(QDataStream &aStream)
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

QString BoolColumn::typeDescription()
{
    QString aTypeDescription;

    aTypeDescription="���. �������� (";

    if (mDefaultValue)
    {
        aTypeDescription.append("X");
    }
    else
    {
        aTypeDescription.append("_");
    }

    aTypeDescription.append(")");

    return aTypeDescription;
}

EColumnType BoolColumn::type()
{
    return ctBool;
}

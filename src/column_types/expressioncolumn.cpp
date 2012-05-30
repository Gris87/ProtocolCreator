#include "src/other/global.h"

ExpressionColumn::ExpressionColumn() : ColumnType()
{
    mAllowModify=false;
}

void ExpressionColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColExpression");

    aStream << QString("Default");
    aStream << mDefaultValue;

    aStream << QString("Modify");
    aStream << mAllowModify;

    aStream << QString("ColEnd");
}

void ExpressionColumn::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Modify")
        {
            aStream >> mAllowModify;
        }
        else
        if (aMagicWord=="ColEnd")
        {
            break;
        }
    }
}

QString ExpressionColumn::typeDescription()
{
    QString aTypeDescription;

    aTypeDescription="Выражение (";

    if (mAllowModify)
    {
        aTypeDescription.append("X");
    }
    else
    {
        aTypeDescription.append("_");
    }

    aTypeDescription.append(
                            "; "+
                            "\""+mDefaultValue+"\")"
                           );

    return aTypeDescription;
}

EColumnType ExpressionColumn::type()
{
    return ctExpression;
}

#include "src/other/global.h"

ExpressionColumn::ExpressionColumn() : ColumnType()
{
}

void ExpressionColumn::saveToStream(QDataStream &aStream)
{
    aStream << QString("ColExpression");

    aStream << QString("Default");
    aStream << mDefaultValue;

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
        if (aMagicWord=="ColEnd")
        {
            break;
        }
    }
}

QString ExpressionColumn::typeDescription()
{
    return "Выражение (\""+mDefaultValue+"\")";
}

EColumnType ExpressionColumn::type()
{
    return ctExpression;
}

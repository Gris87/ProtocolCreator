#include "src/other/global.h"

ColumnType::ColumnType()
{
}

void ColumnType::saveToStream(QDataStream &aStream)
{
    // Nothing
}

void ColumnType::loadFromStream(QDataStream &aStream)
{
    // Nothing
}

QString ColumnType::typeDescription()
{
    return "";
}

EColumnType ColumnType::type()
{
    return ctNone;
}

#ifndef EXPRESSIONCOLUMN_H
#define EXPRESSIONCOLUMN_H

#include <QDataStream>

#include "src/column_types/columntype.h"

class ExpressionColumn: public ColumnType
{
public:
    QString mDefaultValue;

    explicit ExpressionColumn();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    QString typeDescription();
    EColumnType type();
};

#endif // EXPRESSIONCOLUMN_H
#ifndef INTEGERCOLUMN_H
#define INTEGERCOLUMN_H

#include <QDataStream>

#include "src/column_types/columntype.h"

class IntegerColumn: public ColumnType
{
public:
    double mDefaultValue;
    bool mIsAutoInc;
    QString mPrefix;
    QString mPostfix;

    explicit IntegerColumn();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);
};

#endif // INTEGERCOLUMN_H

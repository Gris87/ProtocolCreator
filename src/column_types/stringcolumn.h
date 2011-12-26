#ifndef STRINGCOLUMN_H
#define STRINGCOLUMN_H

#include <QDataStream>

#include "src/column_types/columntype.h"

class StringColumn: public ColumnType
{
public:
    QString mDefaultValue;

    explicit StringColumn();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);
};

#endif // STRINGCOLUMN_H
